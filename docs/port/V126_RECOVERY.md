# v126 recovery branch

This branch rebuilds the broken v124/v125 integration around the last known-good v123 vanilla asset-loading behavior while keeping the current v1.2.0 renderer/performance work.

## Stage 1 isolation

- Vanilla Zero Hour BIG/INI discovery is restored to the v123 implementation.
- Vanilla loose-file lookup/directory enumeration is restored to the v123 implementation.
- `GENERALSX_MOD_DIR` interception is removed from native file access.
- `LaunchActivity` and `GeneralsApplication` are removed for this stage so no process-wide mod environment can leak into a vanilla launch.
- The modern Setup UI is retained, including Vulkan, GLES3 and GLES3+ANGLE backend selection.
- Android validation version is 126.

## Validation gates

A green GitHub Actions build is necessary but is not considered proof that the recovery works. Before this branch can be merged or released:

1. Install the generated v126 APK over the current build and use the same game-data folder that works with v123.
2. Launch vanilla Zero Hour and reach the main menu without `Science ... not recognized`, `CommandSet.ini` parser failures, or the generic `Technical Difficulties` dialog.
3. Start a skirmish and confirm normal unit/science/command definitions load.
4. Verify Vulkan launch first. Then smoke-test native GLES3 and GLES3+ANGLE from the retained Setup renderer selector.
5. Relaunch the app after changing renderers to confirm the selected backend persists and vanilla BIG/INI ordering remains unchanged.
6. Do not reintroduce mod-folder support until all vanilla checks above pass.

## Reintroduction order after vanilla passes

1. Keep vanilla archive enumeration untouched.
2. Reapply safe upstream filesystem/parser fixes independently from mod support.
3. Add loose-file mod overlay as a separately scoped read-only resolver.
4. Add mod BIG loading after all base archives, without changing how the base archive set is discovered.
5. Restore mod-selection UI only after the native overlay passes device tests.

## Baseline

Known-good vanilla reference commit: `006e1f3130c069a5369d24b92f55b365a05c3a1c` (`v123`).

The v124/v125 mod-overlay path is intentionally excluded from Stage 1. No recovery stage is considered releasable solely because CI succeeds.
