// SPDX-License-Identifier: MIT
pragma solidity >=0.5.0 <0.9.0;

import "./interfaces/filesystem_interface.sol";
import "./interfaces/truebit_interface.sol";

contract TruebitTaskPipeContract {

    event NewTask(bytes data);
    event FinishedTask(bytes data, bytes32 result);

    uint internal nonce;
    TrueBit internal truebit;
    Filesystem internal filesystem;
    TRU internal tru;

    bytes32 internal codeFileID;

    mapping (bytes => bytes32) internal string_to_file;
    mapping (bytes32 => bytes) internal task_to_string;
    mapping (bytes => bytes32) internal result;

    uint8 internal memsize;
    uint internal blocklimit;

    constructor(address tb, address tru_, address fs, bytes32 _codeFileID, uint _blocklimit) {
        truebit = TrueBit(tb);
        tru = TRU(tru_);
        filesystem = Filesystem(fs);
        codeFileID = _codeFileID;
        blocklimit = _blocklimit;
    }

    // this is an axiliary function for makeTaskID
    function submitFileData(bytes memory data) private returns (bytes32) {
        uint num = nonce;
        nonce++;

        emit NewTask(data);

        bytes32 inputFileID = filesystem.createFileFromBytes("input.data", num, data);
        string_to_file[data] = inputFileID;
        filesystem.addToBundle(num, inputFileID);

        bytes32[] memory empty = new bytes32[](0);
        filesystem.addToBundle(num, filesystem.createFileFromArray("output.data", num+1000000000, empty, 0));

        filesystem.finalizeBundle(num, codeFileID);
        bytes32 bundleID = filesystem.calculateId(num);
        return bundleID;
    }

    // call this first
    function makeTaskID (bytes calldata data) external returns (bytes32) {
        bytes32 bundleID = submitFileData(data);
        tru.approve(address(truebit), 9 ether);
        truebit.makeDeposit(9 ether);
        bytes32 taskID = truebit.createTaskId(bundleID, 10 ether, 2 ether, 6 ether, 1 ether, blocklimit);
        truebit.requireFile(taskID, filesystem.hashName("output.data"), 0); // 0: eth_bytes, 1: contract, 2: ipfs
        task_to_string[taskID] = data;
        return taskID;
    }

    // call this after makeTaskID
    function emitTask (bytes32 taskID) external payable {
        (bool success,) = address(truebit).call{value:truebit.PLATFORM_FEE_TASK_GIVER()}(abi.encodeWithSignature("submitTask(bytes32)", taskID));
        require(success, "Emit Task Failed!");
    }

    // this is the callback name
    function solved(bytes32 id, bytes32[] memory files) public {
        // could check the task id
        require(TrueBit(msg.sender) == truebit);
        bytes32[] memory arr = filesystem.getBytesData(files[0]);
        result[task_to_string[id]] = arr[0];
        emit FinishedTask(task_to_string[id], arr[0]);
    }
}
