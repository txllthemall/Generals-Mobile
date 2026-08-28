<div align="center">

# Generals Mobile

### Command & Conquer: Generals — Zero Hour for Android

**A mobile-first community port focused on making Zero Hour practical to play on modern Android devices.**

[![Android Build](https://github.com/txllthemall/GeneralsZH-Android-Adreno840-Allocation-Fix/actions/workflows/build-android.yml/badge.svg)](https://github.com/txllthemall/GeneralsZH-Android-Adreno840-Allocation-Fix/actions/workflows/build-android.yml)
[![Release](https://img.shields.io/badge/release-v122-3DDC84?logo=android&logoColor=white)](https://github.com/txllthemall/GeneralsZH-Android-Adreno840-Allocation-Fix/releases)
[![Android](https://img.shields.io/badge/platform-Android-3DDC84?logo=android&logoColor=white)](#requirements)
[![ARM64](https://img.shields.io/badge/architecture-ARM64-0091BD?logo=arm&logoColor=white)](#requirements)
[![Vulkan](https://img.shields.io/badge/renderer-Vulkan%20%2F%20DXVK-AC162C?logo=vulkan&logoColor=white)](#features)
[![License](https://img.shields.io/badge/license-GPL--3.0-blue.svg)](LICENSE.md)

<img src="https://github.com/user-attachments/assets/aeaf6692-36e6-40c8-b9f8-8066d014ec4b" alt="Command & Conquer Generals: Zero Hour running on Android" width="820">

</div>

> [!IMPORTANT]
> Generals Mobile contains engine source code only. It does **not** distribute Command & Conquer game data, artwork, music, videos, or other proprietary assets. A legally owned copy of *Command & Conquer: Generals — Zero Hour* is required for game data.

## What is Generals Mobile?

Generals Mobile started from the Android work around GeneralsX / GeneralsZH-Android-Port, but its scope has grown far beyond a device-specific crash fix. The project now maintains an Android-first Zero Hour experience with its own mobile controls, input improvements, rendering and audio fixes, Android integration, and real-device stability work.

The goal is simple: **make Zero Hour feel like an actual Android game instead of a desktop executable awkwardly running on a phone.**

## Features

- **Native Android ARM64 build**
- **Vulkan rendering through DXVK**
- **Touch controls designed for Zero Hour**
- **In-game touch button editor**
- **Gamepad support**
- **Configurable cursor and mobile input behavior**
- **Control groups and selection improvements for mobile input**
- **Fullscreen and Android gesture-navigation fixes**
- **OpenAL audio fixes**
- **Android-specific DXVK allocation stability fixes**
- **Optional custom Vulkan driver loading support**
- **Automated Android APK builds through GitHub Actions**

## v122

The current Android generation is **version code 122**. It supersedes the old v118 stability build and represents the current mobile-focused codebase.

[**Open Releases**](https://github.com/txllthemall/GeneralsZH-Android-Adreno840-Allocation-Fix/releases)

### Highlights since the original stability fork

The project has expanded with mobile UX work including editable touch controls, gamepad input, cursor customization, Android fullscreen/gesture fixes, selection and group-control improvements, alongside the existing Vulkan/DXVK and OpenAL stability work.

## Stability work

### DXVK allocation crash

The Android DXVK path could corrupt its resource-allocation pool and repeatedly report `DxvkResourceAllocationPool: corrupted free list head`. Generals Mobile carries an Android-specific fallback that avoids the affected pooled free-list path.

### Qualcomm / Adreno

The project was extensively developed and tested on modern Snapdragon hardware, including **Adreno 840**. Adreno 840 is a validated development target, not a limitation of the project. Other Vulkan-capable ARM64 Android devices are welcome and compatibility reports are useful.

### Audio

OpenAL source and buffer lifecycle fixes prevent stale buffers from remaining attached during cache eviction and improve stability with Zero Hour audio on Android.

## Requirements

- 64-bit Android device (`arm64-v8a`)
- Vulkan-capable GPU and driver
- A legally owned Zero Hour installation for game data
- Sufficient free storage for the APK, native libraries and game files

No root access is required for the normal installation path.

## Build

Android is the supported target of this project. The canonical build path is `.github/workflows/build-android.yml`.

1. Open **Actions → Build Android APK**.
2. Run the `android-vulkan` build.
3. Set the required Android version code and version name.
4. Download the generated APK artifact or publish it as a release.

The workflow builds the engine, Android native dependencies and DXVK integration before packaging the APK.

## Project scope

| Area | Status |
|---|---|
| Android ARM64 | **Primary platform** |
| Touch UI / controls | **Active development** |
| Gamepad | **Supported** |
| Vulkan / DXVK | **Active** |
| OpenAL audio | **Active** |
| Adreno 840 | **Tested** |
| Other ARM64 Vulkan devices | Community testing needed |
| Desktop releases | **Not a project target** |

## Reporting bugs

When opening an issue, include:

- phone model and SoC/GPU
- Android version and ROM
- stock or custom Vulkan driver
- Generals Mobile version code
- exact reproduction steps
- Android log around the failure

Do not upload proprietary game files.

## Project lineage and credits

Generals Mobile stands on substantial open-source work by the Command & Conquer community. It builds on the released C&C source code, GeneralsX / Thyme work and the Android porting work of projects such as [MYSOREZ/GeneralsZH-Android-Port](https://github.com/MYSOREZ/GeneralsZH-Android-Port).

Those projects and their contributors remain credited for the foundations Generals Mobile inherited. The Android-specific direction, mobile UX and subsequent integration work are maintained here as Generals Mobile.

Command & Conquer and related names are trademarks of Electronic Arts Inc. This is an unofficial community project and is not endorsed by or affiliated with Electronic Arts.

Source code is distributed under [LICENSE.md](LICENSE.md). Bundled third-party dependencies retain their respective licenses.
