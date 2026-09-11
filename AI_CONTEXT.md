# Generals Mobile Context for Coding Agents

This repository produces one supported product: an ARM64 Android APK for Zero Hour.
Desktop, Linux, macOS, iOS, Flatpak, AppImage and MinGW builds are intentionally out
of scope.

## Current State

- Repository: `txllthemall/Generals-Mobile`
- Build workflow: `.github/workflows/build-android.yml`
- CMake preset: `android-vulkan`
- Package id: `com.generalsx.zerohour`
- Latest published generation before new changes: v122
- Minimum Android API: 28
- Native ABI: `arm64-v8a`

## Mobile Features

- Editable touch controls and hotkeys.
- SDL3 gamepad input with a visible configurable cursor.
- Both landscape orientations.
- Android task-navigation/fullscreen fixes.
- DXVK Vulkan rendering with Adreno and Mali compatibility patches.
- Android allocation-pool workaround for the observed Adreno free-list crash.
- OpenAL lifecycle fixes and Android setup/log tooling.

## Important Boundaries

- CI proves that an APK was built and structurally checked; it does not prove
  controller feel, cursor behavior, audio quality, rotation, or long-session stability.
- The NDK toolchain name `aarch64-linux-android` and Unix compatibility sources are
  required Android implementation details.
- Windows-named compatibility headers emulate APIs used by the original engine and
  are required by the Android compile graph.
- The repository must not contain EA game data or checked-in APK binaries.

## Safe Change Flow

1. Inspect the existing integration point.
2. Patch only the Android path or shared code required by it.
3. Update `docs/DEV_BLOG/2026-08-DIARY.md`.
4. Run cheap local syntax/static checks.
5. Commit and push.
6. Dispatch `build-android.yml` with a higher version code.
7. Inspect every failed step and iterate on the same workflow.
8. Publish the verified APK as a GitHub Release and record its SHA-256.
