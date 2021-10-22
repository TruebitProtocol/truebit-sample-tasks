const fs = require("fs");
const Web3 = require("web3");
const net = require("net");
const getNetwork = require("truebit-util").getNetwork;

// Network configuration
const web3 = new Web3("/root/.ethereum/goerli/geth.ipc", net);
//const web3 = new Web3('http://localhost:8545')

let account, fileSystem, mapReduceSubmitter;
let timeout = async (ms) =>
  new Promise((resolve, reject) => setTimeout(resolve, ms));

async function main() {
  // Set account for Task Submitter
  let accounts = await web3.eth.getAccounts();
  account = accounts[1];
  let networkName = await getNetwork(web3);

  // Get Task Owner artifacts
  const artifacts = JSON.parse(
    fs.readFileSync("public/" + networkName + ".json")
  );

  // Read task input
  mapReduceSubmitter = new web3.eth.Contract(
    artifacts.mapReduce.abi,
    artifacts.mapReduce.address
  );
  const len = process.argv?.length || 0;
  let str = "";
  for (let i = 2; i < len; i++) {
      const element = process.argv[i];
      str += element + '\n';
  }
  console.log("Doing MapReduce on", str);
  let dta = new Buffer.from(str);

  // Deposit task fees
  let tru = new web3.eth.Contract(artifacts.tru.abi, artifacts.tru.address);
  await tru.methods
    .transfer(mapReduceSubmitter.options.address, web3.utils.toWei("9", "ether"))
    .send({ from: account, gas: 200000, gasPrice: web3.gp });
  while ((await tru.methods.balanceOf(account).call({ from: account })) < 9)
    await timeout(1000);
  console.log("Paid TRU task fee");

  // Create Task ID
  let taskID = await mapReduceSubmitter.methods
    .makeTaskID(dta)
    .call({ from: account });
  console.log("TaskID:", taskID);
  await mapReduceSubmitter.methods
    .makeTaskID(dta)
    .send({ gas: 2000000, from: account, gasPrice: web3.gp });

  // Broadcast task
  let IncentiveLayer = new web3.eth.Contract(
    artifacts.incentiveLayer.abi,
    artifacts.incentiveLayer.address
  );
  console.log(
    await IncentiveLayer.methods.taskParameters(taskID).call({ from: account })
  ); // Debug (optional)
  let platformFee = await IncentiveLayer.methods
    .PLATFORM_FEE_TASK_GIVER()
    .call({ from: account });
  await mapReduceSubmitter.methods.emitTask(taskID).send({
    gas: 100000,
    from: account,
    value: platformFee,
    gasPrice: web3.gp,
  });
  console.log("Task submitted.  Waiting for solution...");

  // Wait for solution
  let solution =
    "0x0000000000000000000000000000000000000000000000000000000000000000";
  while (
    solution ==
    "0x0000000000000000000000000000000000000000000000000000000000000000"
  ) {
    await timeout(1000);
    solution = await mapReduceSubmitter.methods
      .mapReduce(dta)
      .call({ from: account });
  }
  console.log("Got solution", solution);
}

main();
