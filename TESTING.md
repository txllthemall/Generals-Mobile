# Android Validation

## Build validation

Run `.github/workflows/build-android.yml` with the `android-vulkan` preset. The job
must finish native compilation, APK packaging and its ABI/ELF/package checks.

## Device validation

Install with `adb install -r <apk>` only when the APK version code is newer than the
installed build. Verify at minimum:

- launch through the animated main menu;
- touch selection, camera scrolling and building placement;
- Home/recents navigation and return to the game;
- both landscape orientations;
- gamepad cursor, clicks, hotkeys and configuration;
- music, speech and repeated combat audio;
- a representative skirmish and a longer stability session;
- logcat contains no fatal Android exception, native signal, allocation-pool
  corruption, Vulkan device loss or OpenAL exception flood.

CI success is never a substitute for these device checks.
