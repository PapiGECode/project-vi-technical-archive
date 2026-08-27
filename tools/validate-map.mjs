// Project VI Technical Archive — Lead developer: Cyberleek
import { readFile } from 'node:fs/promises';
import path from 'node:path';

const mapPath = path.resolve(process.argv[2] ?? 'config/maps/coastal_testbed.json');
const map = JSON.parse(await readFile(mapPath, 'utf8'));
const errors = [];

const assert = (condition, message) => {
  if (!condition) errors.push(message);
};

assert(map.$schema === 'pvi.map-profile/v1', 'unsupported or missing $schema');
assert(typeof map.id === 'string' && map.id.length > 0, 'map.id is required');
assert(map.coordinateSystem === 'right-handed-z-up', 'coordinateSystem must be right-handed-z-up');
assert(map.units === 'meters', 'units must be meters');

const streaming = map.streaming ?? {};
const cells = Array.isArray(streaming.cells) ? streaming.cells : [];
assert(cells.length > 0, 'streaming.cells must contain at least one cell');
assert(streaming.defaultLoadRadius > 0, 'defaultLoadRadius must be positive');
assert(streaming.defaultUnloadRadius >= streaming.defaultLoadRadius, 'unload radius must be >= load radius');

const ids = new Set();
for (const cell of cells) {
  assert(Number.isInteger(cell.id), `cell ${cell.name ?? '<unnamed>'} has an invalid id`);
  assert(!ids.has(cell.id), `duplicate cell id ${cell.id}`);
  ids.add(cell.id);
  assert(Array.isArray(cell.center) && cell.center.length === 3 && cell.center.every(Number.isFinite), `cell ${cell.id} center must be a numeric vec3`);
  assert(Array.isArray(cell.neighbors), `cell ${cell.id} neighbors must be an array`);
}

for (const cell of cells) {
  for (const neighbor of cell.neighbors ?? []) {
    assert(ids.has(neighbor), `cell ${cell.id} references missing neighbor ${neighbor}`);
    const reverse = cells.find((candidate) => candidate.id === neighbor)?.neighbors?.includes(cell.id);
    assert(reverse, `neighbor link ${cell.id} -> ${neighbor} is not reciprocal`);
  }
}

if (cells.length) {
  const visited = new Set();
  const queue = [cells[0].id];
  while (queue.length) {
    const id = queue.shift();
    if (visited.has(id)) continue;
    visited.add(id);
    const cell = cells.find((candidate) => candidate.id === id);
    queue.push(...(cell?.neighbors ?? []).filter((neighbor) => !visited.has(neighbor)));
  }
  assert(visited.size === cells.length, `streaming graph is disconnected (${visited.size}/${cells.length} reachable)`);
}

for (const spawn of map.spawnPoints ?? []) {
  assert(typeof spawn.id === 'string' && spawn.id.length > 0, 'spawn point id is required');
  assert(Array.isArray(spawn.position) && spawn.position.length === 3 && spawn.position.every(Number.isFinite), `spawn ${spawn.id} position must be a numeric vec3`);
  assert(Number.isFinite(spawn.headingDegrees), `spawn ${spawn.id} headingDegrees must be numeric`);
}

if (errors.length) {
  console.error(`map validation failed: ${path.relative(process.cwd(), mapPath)}`);
  errors.forEach((error) => console.error(`  - ${error}`));
  process.exitCode = 1;
} else {
  console.log(`map valid: ${path.relative(process.cwd(), mapPath)}`);
  console.log(`${cells.length} streaming cells · ${(map.spawnPoints ?? []).length} spawn points · connected graph`);
}
