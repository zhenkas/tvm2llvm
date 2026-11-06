// Original inspection fixture, compiled with the same solc/settings as Bench.sol.
const fs = require('node:fs');
const crypto = require('node:crypto');
const solc = require('solc');
const source = fs.readFileSync('examples/Lookups.sol', 'utf8');
const settings = {
  optimizer: { enabled: true, runs: 200 },
  evmVersion: 'istanbul',
  metadata: { bytecodeHash: 'none' },
  outputSelection: { '*': { '*': ['abi', 'storageLayout', 'evm.methodIdentifiers',
    'evm.deployedBytecode.object', 'evm.deployedBytecode.sourceMap',
    'evm.deployedBytecode.opcodes'] } }
};
const output = JSON.parse(solc.compile(JSON.stringify({
  language: 'Solidity', sources: { 'Lookups.sol': { content: source } }, settings
})));
for (const error of output.errors || []) {
  console.error(error.formattedMessage);
  if (error.severity === 'error') process.exitCode = 1;
}
if (process.exitCode) process.exit();
const c = output.contracts['Lookups.sol'].Lookups;
fs.writeFileSync('fixtures/Lookups.json', JSON.stringify({
  compiler: solc.version(), settings,
  source_sha256: crypto.createHash('sha256').update(source).digest('hex'),
  abi: c.abi, storage_layout: c.storageLayout,
  selectors: c.evm.methodIdentifiers,
  runtime: c.evm.deployedBytecode.object,
  source_map: c.evm.deployedBytecode.sourceMap,
  opcodes: c.evm.deployedBytecode.opcodes
}, null, 2) + '\n');
console.log('Lookups runtime bytes', c.evm.deployedBytecode.object.length / 2);
