---
applyTo: "**"
---

# Commit Instructions

Use a concise conventional commit title. Preferred scopes include `android`,
`controls`, `gamepad`, `audio`, `dxvk`, `cmake`, `ci` and `docs`.

Examples:

- `fix(android): keep task navigation available`
- `feat(gamepad): add configurable virtual cursor`
- `fix(dxvk): avoid Android allocation-pool recycling`
- `build(android): repair APK font staging`
- `chore: remove unsupported desktop build infrastructure`

Keep generated APKs and proprietary game assets out of commits. Update the current
development diary before committing. If a change is made inside the DXVK submodule,
commit it in that repository and then update the pinned gitlink separately.
