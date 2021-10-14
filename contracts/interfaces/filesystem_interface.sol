// SPDX-License-Identifier: MIT
pragma solidity >=0.5.0 <0.9.0;

interface Filesystem {
   function createFileFromArray(string calldata name, uint nonce, bytes32[] calldata arr, uint sz) external returns (bytes32);
   function createFileFromBytes(string calldata name, uint nonce, bytes calldata arr) external returns (bytes32);
   function getBytesData(bytes32 id) external view returns (bytes32[] memory);

   function calculateId(uint nonce) external view returns (bytes32);
   function addToBundle(uint nonce, bytes32 file_id) external;
   function finalizeBundle(uint nonce, bytes32 codeFileID) external;
   function hashName(string calldata name) external returns (bytes32);
}
