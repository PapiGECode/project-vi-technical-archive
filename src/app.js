const grid = document.querySelector('#assetGrid');
const inspector = document.querySelector('#inspector');
const terminal = document.querySelector('#terminal');
const scanButton = document.querySelector('#scanButton');
const integrityState = document.querySelector('#integrityState');

const formatBytes = (bytes) => {
  const units = ['B', 'KB', 'MB'];
  let value = bytes;
  let index = 0;
  while (value >= 1024 && index < units.length - 1) { value /= 1024; index += 1; }
  return `${value.toFixed(index ? 1 : 0)} ${units[index]}`;
};

const digest = async (buffer) => {
  const hash = await crypto.subtle.digest('SHA-256', buffer);
  return [...new Uint8Array(hash)].map((byte) => byte.toString(16).padStart(2, '0')).join('');
};

const showRecord = (asset, card) => {
  document.querySelectorAll('.asset-card').forEach((item) => item.classList.remove('active'));
  card.classList.add('active');
  inspector.innerHTML = `
    <p class="inspector-label">ASSET INSPECTOR</p>
    <h3>${asset.name}</h3>
    <p>Public promotional reference indexed by the community reconstruction toolchain.</p>
    <ul class="record">
      <li><span>REPOSITORY PATH</span><code>${asset.path}</code></li>
      <li><span>MEDIA / SIZE</span><code>${asset.mediaType} · ${formatBytes(asset.bytes)}</code></li>
      <li><span>SHA-256</span><code>${asset.sha256}</code></li>
      <li><span>CLASSIFICATION</span><code>PUBLIC_PROMOTIONAL_REFERENCE</code></li>
    </ul>`;
};

const manifest = await fetch('data/asset-manifest.json').then((response) => {
  if (!response.ok) throw new Error(`manifest request failed: ${response.status}`);
  return response.json();
});

document.querySelector('#manifestVersion').textContent = manifest.manifestVersion.toUpperCase();
document.querySelector('#assetCount').textContent = String(manifest.assets.length).padStart(2, '0');

manifest.assets.forEach((asset, index) => {
  const card = document.createElement('button');
  card.className = 'asset-card';
  card.innerHTML = `
    <div class="asset-frame"><span class="asset-index">${String(index + 1).padStart(2, '0')}</span><img loading="lazy" src="${asset.path}" alt="${asset.name}"></div>
    <div class="asset-meta"><strong>${asset.name}</strong><span>${formatBytes(asset.bytes)}</span></div>`;
  card.addEventListener('click', () => showRecord(asset, card));
  grid.append(card);
});

scanButton.addEventListener('click', async () => {
  scanButton.disabled = true;
  scanButton.textContent = 'Scanning…';
  integrityState.textContent = 'SCANNING';
  terminal.textContent = '$ browser.integrity.scan --manifest data/asset-manifest.json\n';
  let passed = 0;
  for (const asset of manifest.assets) {
    const response = await fetch(asset.path);
    const hash = await digest(await response.arrayBuffer());
    const ok = hash === asset.sha256;
    if (ok) passed += 1;
    terminal.textContent += `${ok ? '[ OK ]' : '[FAIL]'} ${asset.name}\n`;
  }
  const valid = passed === manifest.assets.length;
  integrityState.textContent = valid ? 'VERIFIED' : 'MISMATCH';
  terminal.textContent += `\n${passed}/${manifest.assets.length} records verified · ${valid ? 'archive clean' : 'integrity mismatch'}`;
  scanButton.textContent = valid ? 'Integrity verified' : 'Scan failed';
  scanButton.disabled = false;
});
