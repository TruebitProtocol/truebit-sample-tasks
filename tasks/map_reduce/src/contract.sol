// SPDX-License-Identifier: MIT
pragma solidity >=0.4.15 <0.9.0;

import "./contracts/truebit_task_pipe_contract.sol";

contract TruebitMapReduceContract is TruebitTaskPipeContract {

    constructor(address tb, address tru_, address fs, bytes32 _codeFileID, uint _blocklimit) TruebitTaskPipeContract(tb, tru_, fs, _codeFileID, _blocklimit) public {}

    function mapReduce(bytes memory data) public view returns (bytes32) {
        return result[data];
    }
}
