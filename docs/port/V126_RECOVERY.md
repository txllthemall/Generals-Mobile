# v126 recovery branch

This branch is rebuilt from the last known-good v123 baseline.

## Recovery rules

- Vanilla Zero Hour BIG/INI discovery stays identical to v123 until device validation passes.
- Upstream v1.2.0 changes are reintroduced in isolated subsystems.
- Renderer, performance, audio and Android lifecycle changes are validated before any mod-overlay work.
- Secondary mod-folder support is reintroduced last and must not alter vanilla archive enumeration.
- Every recovery stage must pass Android CI before the next subsystem is added.

## Baseline

Base commit: `006e1f3130c069a5369d24b92f55b365a05c3a1c` (`v123`).

The v124/v125 BIG/INI and mod-overlay changes are intentionally excluded from the initial recovery baseline.
