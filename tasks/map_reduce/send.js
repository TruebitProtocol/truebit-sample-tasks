const fs = require("fs");
const Web3 = require("web3");
const net = require("net");
const getNetwork = require("truebit-util").getNetwork;
const { exit } = require("process");

// Network configuration
const web3 = new Web3("/root/.ethereum/goerli/geth.ipc", net);
//const web3 = new Web3('http://localhost:8545')

const SOLUTION_BLANK = "0x0000000000000000000000000000000000000000000000000000000000000000";

let account, fileSystem, mapReduceSubmitter;
let timeout = async (ms) =>
  new Promise((resolve, reject) => setTimeout(resolve, ms));

async function main(account_num) {
  // Set account for Task Submitter
  let accounts = await web3.eth.getAccounts();
  account = accounts[account_num];
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
  const len = process.argv.length || 0;
  let str = "";
  for (let i = 3; i < len; i++) {
    const element = process.argv[i];
    str += element.trim() + "\n";
  }
  str = str.trim();
  console.log("Doing MapReduce on:\n%s", str);
  let dta = new Buffer.from(str);

  // Deposit task fees
  let tru = new web3.eth.Contract(artifacts.tru.abi, artifacts.tru.address);
  await tru.methods
    .transfer(
      mapReduceSubmitter.options.address,
      web3.utils.toWei("9", "ether")
    )
    .send({ from: account, gas: 200000, gasPrice: web3.gp });
  while ((await tru.methods.balanceOf(account).call({ from: account })) < 9)
    await timeout(1000);
  console.log("Paid TRU task fee");

  // Create Task ID
  let taskID;
  try {
    taskID = await mapReduceSubmitter.methods
      .makeTaskID(dta)
      .call({ from: account });
  } catch (e) {
    console.log("ERROR: (makeTaskID call) %s", e);
    exit(1);
  }
  console.log("TaskID:", taskID);
  try {
    await mapReduceSubmitter.methods
      .makeTaskID(dta)
      .send({ gas: 2000000, from: account, gasPrice: web3.gp });
  } catch (e) {
    console.log("ERROR: (makeTaskID call) %s", e);
    exit(1);
  }

  // Broadcast task
  let IncentiveLayer = new web3.eth.Contract(
    artifacts.incentiveLayer.abi,
    artifacts.incentiveLayer.address
  );
  const taskParams = await IncentiveLayer.methods
    .taskParameters(taskID)
    .call({ from: account });
  console.log("STATUS:\n%s", JSON.stringify(taskParams)); // Debug (optional)
  let platformFee;
  try {
    platformFee = await IncentiveLayer.methods
      .PLATFORM_FEE_TASK_GIVER()
      .call({ from: account });
  } catch (e) {
    console.log("ERROR: (PLATFORM_FEE_TASK_GIVER call) %s", e);
    exit(1);
  }
  console.log("STATUS:\n\taccount: %s\n\tplatformFee: %s", account, platformFee); // Debug (optional)
  try {
    await mapReduceSubmitter.methods.emitTask(taskID).send({
      gas: 100000,
      from: account,
      value: platformFee,
      gasPrice: web3.gp,
    });
  } catch (e) {
    console.log("ERROR: (emitTask call) %s", e);
    exit(1);
  }
  console.log("Task submitted.  Waiting for solution...");

  // Wait for solution
  let solution = SOLUTION_BLANK;
  while ( solution === SOLUTION_BLANK ) {
    await timeout(1000);
    try {
      solution = await mapReduceSubmitter.methods
        .mapReduce(dta)
        .call({ from: account });
    } catch (e) {
      console.log("ERROR: (mapReduce call) %s", e);
      exit(1);
    }
  }
  console.log("Got solution", solution);
}

const args = process.argv.slice(2);
main(parseInt(args[0], 10) || 0);
