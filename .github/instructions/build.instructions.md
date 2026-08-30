---
applyTo: "cmake/**,CMakeLists.txt,CMakePresets.json,.github/workflows/build-android.yml"
---

# Android Build Instructions

The canonical and only supported configuration is `android-vulkan`.

```bash
git submodule update --init references/fbraz3-dxvk
git -C references/fbraz3-dxvk submodule update --init --depth 1
./scripts/build/android/build-android-zh.sh
./scripts/build/android/package-android-zh.sh
```

CI must use `.github/workflows/build-android.yml`. Extend that workflow when needed;
do not add parallel Android workflows.

DXVK is built from the pinned local gitlink and the ordered patches in
`cmake/dx8.cmake`. Validate patch ordering and idempotency after changing the list.
The NDK triple contains the string `linux` by design and must not be renamed.
