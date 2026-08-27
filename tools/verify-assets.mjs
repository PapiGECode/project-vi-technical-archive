// Project VI Technical Archive — Lead developer: Cyberleek
import { createHash } from 'node:crypto';
import { readFile } from 'node:fs/promises';
import path from 'node:path';

const root = process.cwd();
const manifest = JSON.parse(await readFile(path.join(root, 'data', 'asset-manifest.json'), 'utf8'));
let failed = 0;

for (const asset of manifest.assets) {
  const contents = await readFile(path.join(root, ...asset.path.split('/')));
  const actual = createHash('sha256').update(contents).digest('hex');
  const valid = actual === asset.sha256 && contents.byteLength === asset.bytes;
  console.log(`${valid ? '[ OK ]' : '[FAIL]'} ${asset.path}`);
  if (!valid) failed += 1;
}

console.log(`\n${manifest.assets.length - failed}/${manifest.assets.length} records verified`);
if (failed) process.exitCode = 1;
