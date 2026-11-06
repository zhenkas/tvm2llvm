// SPDX-License-Identifier: LGPL-3.0-or-later
pragma solidity 0.8.26;
// Deliberately small, original benchmark fixtures. No mainnet state or third-party contracts.
contract Bench {
    mapping(address => uint256) public balances;
    event Transfer(address indexed from, address indexed to, uint256 amount);
    function arithmetic(uint256 n, uint256 x) external pure returns (uint256) {
        unchecked {
            for (uint256 i; i < n; ++i) {
                x = (x * 6364136223846793005 + i) ^ (x >> 13);
            }
        }
        return x;
    }
    function hashLoop(uint256 n, bytes32 x) external pure returns (bytes32) {
        for (uint256 i; i < n; ++i) {
            x = keccak256(abi.encode(x, i));
        }
        return x;
    }
    function transfer(address to, uint256 amount) external returns (uint256) {
        uint256 value = balances[msg.sender];
        require(value >= amount, "insufficient");
        unchecked { balances[msg.sender] = value - amount; }
        balances[to] += amount;
        emit Transfer(msg.sender, to, amount);
        return balances[to];
    }
    function seed(uint256 amount) external { balances[msg.sender] = amount; }
    function gasProbe(uint256 n) external view returns (uint256 before_, uint256 after_) {
        before_ = gasleft();
        uint256 x;
        unchecked { for (uint256 i; i < n; ++i) x = (x * 17) ^ i; }
        after_ = gasleft() + (x & 1);
    }
    function fail(uint256 x) external pure returns (uint256) {
        require(x != 0, "zero");
        return 100 / x;
    }
}
