// SPDX-License-Identifier: LGPL-3.0-or-later
pragma solidity 0.8.26;

// Storage is supplied by the in-memory Rust fixture, not by a deployment/database.
contract Lookups {
    mapping(address => uint256) private balances; // storage slot 0
    uint256[] private values;                    // storage slot 1

    function mapGet(address who) external view returns (uint256) {
        return balances[who];
    }

    function listGet(uint256 index) external view returns (uint256) {
        return values[index];
    }

    function hashBytes(bytes calldata data) external pure returns (bytes32) {
        return keccak256(data);
    }
}
