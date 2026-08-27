<p align="center">
  <img src="assets/reference/official-promo/brand-lockup.png" width="560" alt="Grand Theft Auto VI promotional logo">
</p>

<h1 align="center">GRAND THEFT AUTO VI</h1>

<p align="center"><code>visual archive</code> · <code>asset indexing</code> · <code>integrity tooling</code></p>

<p align="center"><sub>Technical visual archive and asset reconstruction environment. Independent from Rockstar Games and Take-Two Interactive.</sub></p>

## Repository profile

Project VI is a technical visual archive designed to catalog publicly released promotional material and provide a build-style interface for structured asset inspection.

The repository includes deterministic SHA-256 asset cataloging, file-integrity verification tooling, metadata indexing, and a static archive interface for browsing and validating referenced material.

```text
project-vi/
├── assets/reference/official-promo/   # public promotional reference material
├── data/asset-manifest.json           # generated checksums and file metadata
├── src/app.js                         # archive interface and browser verification
├── tools/catalog-assets.mjs           # deterministic manifest generator
├── tools/verify-assets.mjs            # local integrity scanner
└── project.json                       # archive configuration profile
```

## Local operation

Requires Node.js 18 or newer.

```bash
npm run catalog
npm run verify
npm run dev
```

Open `http://localhost:4173` after starting the local server.

## Project maintenance

Project VI is maintained under the **NullCoast** project identity.

Repository maintenance covers archive structure, metadata generation, integrity verification, interface development, and preservation of the indexed reference material.

## Asset notice

Grand Theft Auto, GTA, Rockstar Games, and all associated names, logos, artwork, trademarks, and promotional materials are the property of their respective owners.

Promotional reference material is stored separately under `assets/reference/official-promo` and is excluded from the repository's MIT license.

See [ASSET_NOTICE.md](ASSET_NOTICE.md) for additional information.
