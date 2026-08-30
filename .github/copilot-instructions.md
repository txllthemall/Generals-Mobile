# Generals Mobile Repository Instructions

The only supported target is Android ARM64. Follow `AGENTS.md` and `AI_CONTEXT.md`.

- Use the `android-vulkan` CMake preset.
- Reuse `.github/workflows/build-android.yml`.
- Keep DXVK fixes in `Patches/` and apply them from `cmake/dx8.cmake`.
- Keep touch, gamepad, rotation and Android lifecycle work under `android/` or the
  shared SDL3 implementation used by Android.
- Keep OpenAL and rendering fixes narrowly scoped and evidence-backed.
- Do not introduce desktop, macOS, iOS, Linux distribution, Docker build, Flatpak,
  AppImage or MinGW packaging.
- Do not delete compatibility code based on filenames alone; validate the Android
  build graph first.
- Do not commit APKs or proprietary game data. Attach APKs to GitHub Releases.
- Update the August 2026 development diary before committing.
- Separate CI validation from real-device validation in reports.
