// SPDX-License-Identifier: MIT
pragma solidity >=0.4.15 <0.9.0;

interface TrueBit {
   function createTaskId(bytes32 bundleId, uint minDeposit, uint solverReward, uint verifierTax, uint ownerFee, uint limit) external returns (bytes32);
   function requireFile(bytes32 id, bytes32 hash, /* Storage */ uint8 st) external;
   function submitTask(bytes32 id) external payable;
   function makeDeposit(uint) external returns (uint);
   function PLATFORM_FEE_TASK_GIVER() external view returns (uint);
}

interface TRU {
    function approve(address spender, uint tokens) external returns (bool success);
}
