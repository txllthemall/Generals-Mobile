# Android Build Scripts

The supported scripts are under `scripts/build/android/`:

- `build-android-zh.sh` configures and builds the ARM64 native game and DXVK.
- `package-android-zh.sh` stages native libraries/assets and builds the APK.
- `stage-fonts.sh` downloads the pinned Liberation font archive used by the APK.
- `fetch-turnip.sh` and `fetch-vulkan-validation-layer.sh` stage optional diagnostics.
- `build-local-sandboxed.sh` provides the isolated local validation path.

The canonical remote build is `.github/workflows/build-android.yml` with the
`android-vulkan` preset. APKs belong in workflow artifacts and GitHub Releases, not
in the source tree.
