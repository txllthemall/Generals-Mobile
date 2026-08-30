# Generals Mobile: AI Agent Guide

## Mission

Generals Mobile is an Android-first port of Command & Conquer: Generals — Zero Hour.
The supported product is the ARM64 Android APK. Do not add desktop, macOS, iOS,
Flatpak, AppImage, Docker desktop-build, or MinGW release infrastructure.

## Read First

- `AI_CONTEXT.md`
- `.github/copilot-instructions.md`
- `docs/port/ANDROID_PORT.md`
- `docs/DEV_BLOG/2026-08-DIARY.md`

## Canonical Build

- Preset: `android-vulkan`
- Workflow: `.github/workflows/build-android.yml`
- Native build: `scripts/build/android/build-android-zh.sh`
- APK packaging: `scripts/build/android/package-android-zh.sh`
- DXVK source: pinned `references/fbraz3-dxvk` gitlink plus `Patches/dxvk-*.patch`

Never create a second Android workflow unless the existing one cannot reasonably be
extended. Never claim a device, controller, audio, or gameplay result from CI alone.

## Android Architecture

- `android/`: Gradle application, Setup and control editors, SDL Java patches.
- `GeneralsMD/`: Zero Hour engine and game code.
- `Core/`: shared engine, SDL3, OpenAL and platform abstractions.
- `Generals/Code/CompatLib/`: compatibility headers still consumed by the Android
  build. Their Windows-style names are API emulation, not a Windows target.
- `cmake/`: Android dependency and DXVK integration.

Android uses Unix-style source paths and the NDK triple `aarch64-linux-android`.
Do not delete a file merely because its name contains `unix`, `linux`, or `windows`;
first prove that the `android-vulkan` graph does not consume it.

## Working Rules

1. Keep changes minimal and Android-scoped.
2. Preserve retail game-data compatibility and deterministic gameplay.
3. Do not commit proprietary game archives, videos, music, or prebuilt APKs.
4. Publish APKs through GitHub Releases, not in the Git tree.
5. Keep code, comments and documentation in English.
6. Use conventional commit titles.
7. Update the current monthly file in `docs/DEV_BLOG/` before committing.
8. Verify shell syntax, CMake configuration and the existing Android workflow.
9. Treat CI success as build validation only; list real-device checks separately.

## Release Checklist

Use `docs/port/RELEASE_CHECKLIST.md`. A release needs a monotonically increasing
Android version code, a matching `v<version_name>` tag, a downloadable APK asset,
and recorded checksums.
