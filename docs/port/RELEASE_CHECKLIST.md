# Android Release Checklist

- [ ] Existing `build-android.yml` completes successfully.
- [ ] Android version code is higher than every previously published build.
- [ ] Version name and `v<version_name>` tag match.
- [ ] APK contains only `arm64-v8a` native libraries and passes workflow checks.
- [ ] No `*.big`, `*.bik`, proprietary game data, credentials or signing secrets are tracked.
- [ ] APK is uploaded to GitHub Releases, not committed to the repository.
- [ ] SHA-256 and APK size are recorded in the release notes.
- [ ] Device checks are listed explicitly; unperformed checks remain marked unverified.
- [ ] README version badge and release section are current.
