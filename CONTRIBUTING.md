# Contributing to Generals Mobile

Generals Mobile accepts Android-focused improvements for Zero Hour: touch and
gamepad controls, Android lifecycle/UI, Vulkan/DXVK compatibility, OpenAL audio,
performance, stability and device compatibility.

Desktop platform ports and packaging are outside this repository's scope.

Before opening a pull request:

1. Keep the change focused and explain the real device or log evidence behind it.
2. Use the existing `android-vulkan` preset and Android workflow.
3. Do not include proprietary game data or APK binaries.
4. Update the current development diary for user-visible or build changes.
5. Report CI checks separately from any real-device tests performed.

Use conventional commit titles such as `fix(android): ...`, `fix(dxvk): ...`,
`feat(controls): ...`, `build(android): ...`, or `docs: ...`.
