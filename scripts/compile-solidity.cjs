// Original fixture build; the runtime bytecode is compiled once before benchmarking.
const fs = require('node:fs');
const crypto = require('node:crypto');
const solc = require('solc');
const source = fs.readFileSync('contracts/Bench.sol', 'utf8');
const settings = {
  optimizer: { enabled: true, runs: 200 },
  evmVersion: 'istanbul',
  metadata: { bytecodeHash: 'none' },
  outputSelection: { '*': { '*': ['abi', 'evm.deployedBytecode.object'] } }
};
const output = JSON.parse(solc.compile(JSON.stringify({
  language: 'Solidity', sources: { 'Bench.sol': { content: source } }, settings
})));
for (const error of output.errors || []) {
  console.error(error.formattedMessage);
  if (error.severity === 'error') process.exitCode = 1;
}
if (process.exitCode) process.exit();
const contract = output.contracts['Bench.sol'].Bench;
fs.writeFileSync('fixtures/Bench.json', JSON.stringify({
  compiler: solc.version(), settings,
  source_sha256: crypto.createHash('sha256').update(source).digest('hex'),
  abi: contract.abi, runtime: contract.evm.deployedBytecode.object
}, null, 2) + '\n');
console.log('fixture bytecode bytes', contract.evm.deployedBytecode.object.length / 2);
