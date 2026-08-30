---
applyTo: "**/*.{cpp,h,hpp,c}"
---

# C and C++ Instructions

- Preserve gameplay determinism and retail data compatibility.
- Keep Android/SDL3/OpenAL/DXVK changes in platform or device layers whenever possible.
- Do not add native desktop platform APIs to game logic.
- Match the surrounding legacy style and avoid unrelated refactors.
- Write code, comments and identifiers in English.
- Add the established `// GeneralsX @keyword ...` annotation for user-facing fixes.
- Treat Windows-named compatibility APIs as part of the emulation surface; do not
  remove them without proving the Android target no longer consumes them.
- Validate weak-memory-ordering and object-lifetime changes carefully on ARM64.
