import { createHash } from 'node:crypto';
import { readdir, readFile, writeFile } from 'node:fs/promises';
import path from 'node:path';

const root = process.cwd();
const assetDirectory = path.join(root, 'assets', 'reference', 'official-promo');
const files = (await readdir(assetDirectory)).filter((name) => name.toLowerCase().endsWith('.png')).sort();
const assets = [];

for (const name of files) {
  const absolutePath = path.join(assetDirectory, name);
  const contents = await readFile(absolutePath);
  assets.push({
    name,
    path: `assets/reference/official-promo/${name}`,
    mediaType: 'image/png',
    bytes: contents.byteLength,
    sha256: createHash('sha256').update(contents).digest('hex')
  });
}

const manifest = {
  manifestVersion: 'sha256-v1',
  profile: 'project-vi-community',
  classification: 'public-promotional-reference',
  assets
};

await writeFile(path.join(root, 'data', 'asset-manifest.json'), `${JSON.stringify(manifest, null, 2)}\n`);
console.log(`cataloged ${assets.length} assets`);
