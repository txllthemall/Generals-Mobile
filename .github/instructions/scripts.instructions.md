---
applyTo: "scripts/**"
---

# Script Instructions

Runtime build and packaging scripts live in `scripts/build/android/`.

- Use Bash with `set -euo pipefail`.
- Resolve paths relative to the script and repository, not a developer's machine.
- Pin and checksum downloaded build inputs.
- Keep secrets and personal paths out of the repository.
- Do not add desktop, Docker, macOS, iOS, Flatpak, AppImage or MinGW build scripts.
- Package APKs through Gradle and publish them as workflow artifacts/releases.
- Keep device installation optional through an explicit `--install` argument.
