<p align="center">
  <img src="assets/reference/official-promo/brand-lockup.png" width="560" alt="Grand Theft Auto VI promotional logo">
</p>

<h1 align="center">PROJECT VI // COMMUNITY RECONSTRUCTION</h1>

<p align="center"><code>visual archive</code> · <code>asset indexing</code> · <code>integrity tooling</code></p>

<p align="center"><sub>Unofficial fan-made technical reconstruction. Not affiliated with Rockstar Games or Take-Two Interactive.</sub></p>

## Repository profile

Project VI is a community-maintained visual study that catalogs publicly released promotional artwork and experiments with a build-style asset browser. The repository contains an actual SHA-256 catalog generator, integrity verification tooling, and a static archive interface.

```text
project-vi/
├── assets/reference/official-promo/   # supplied public promotional references
├── data/asset-manifest.json           # generated checksums and file metadata
├── src/app.js                         # archive interface and browser verification
├── tools/catalog-assets.mjs           # deterministic manifest generator
├── tools/verify-assets.mjs            # local integrity scanner
└── project.json                       # reconstruction profile
```

## Local operation

Requires Node.js 18 or newer.

```bash
npm run catalog
npm run verify
npm run dev
```

Open `http://localhost:4173` after starting the local server.

## Maintainer

Maintained under the fictional community alias **NullCoast**. The alias does not represent or imply affiliation with any real leaker, employee, studio, or publisher.

## Asset notice

Grand Theft Auto, GTA, Rockstar Games, and associated artwork and marks belong to their respective owners. Promotional references are separated under `assets/reference/official-promo` and are not covered by the repository's MIT license. See [ASSET_NOTICE.md](ASSET_NOTICE.md).
