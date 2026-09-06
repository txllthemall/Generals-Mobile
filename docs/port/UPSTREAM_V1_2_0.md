# Upstream v1.2.0 integration

Source: [MYSOREZ/GeneralsZH-Android-Port v1.2.0](https://github.com/MYSOREZ/GeneralsZH-Android-Port/releases/tag/v1.2.0).
The imported snapshot is commit `90b82c1ce59fbce03a0a47f1fccc1365b39eacf3`.
The common ancestor with Generals Mobile is `8f521e966fd639f18caf57bed245f85b30c08836`;
the snapshot includes the cumulative source changes across 104 upstream commits.
Generals Mobile was based on `006e1f3130c069a5369d24b92f55b365a05c3a1c` before this integration.

## Included changes

- Native D3D8-to-GLES3 renderer, bundled ARM64 ANGLE, shared backend selection,
  and a translated Setup picker for Vulkan, GLES and GLES+ANGLE.
- Dynamic vertex/index lock-range handling and NOOVERWRITE synchronization;
  VAO, render-state, uniform and texture-binding caches; camera UBO and glyph-atlas reuse.
- D3D8 render-state defaults, MULTIPLYADD/LERP texture stages, stencil reference
  and mask handling, DXT/BC1-3 software decoding, and GLES texture/FBO lifecycle fixes.
- Correct GLES pixel alignment, partial viewports, pillarbox orientation,
  ANGLE pre-rotation behavior, and GLES-specific heat-distortion suppression.
- Dynamic shadow LOD at the lowest FPS tier, opaque score-screen render skipping,
  cheaper Android FFmpeg scaling, and collapsed group-panel work avoidance.
- Folder-integrity diagnostics with nested INI archive discovery and prominent status.
- GeneralsOnline HTTP/transport error details, alternate API endpoint fallback,
  and retention of cached sessions after transient network failures.
- DXVK adapter diagnostics and the missing DxvkEvent refcount acquire fence;
  corrected native crash offsets and additional rendering performance diagnostics.
- Smaller CI APKs through native-symbol stripping, with DXVK libraries excluded.

The snapshot already reverts the unsuccessful GPU-instancing experiment and
temporary landscape-orientation experiment. Their intermediate behavior is not enabled.

## Generals Mobile adaptations

- Vulkan remains the default for an absent, empty or invalid backend setting.
  Both native decisions use one resolver; Setup uses the same Vulkan fallback.
  Explicit picker choices and developer environment overrides remain available.
- Version name/code advance from `123` to `124`; the upstream `10200` version
  scheme is not substituted for this project's release sequence.
- SDL still initializes the gamepad subsystem. The configurable cursor, hotkeys,
  touch editor, both landscape orientations and existing audio fixes are retained.
- The SDL system-bar callback keeps this project's gesture-preserving behavior
  and also adopts upstream's window-focus guard.
- `cmake/dx8.cmake` retains the Android-only build and the Adreno allocation-pool
  workaround. The new refcount patch follows the existing refcount audit.
- ANGLE binaries retain the exact upstream contents. Packaging requires both
  libraries and includes the ANGLE license. Optional missing libraries do not
  turn the CI symbol-stripping loop into a spurious failure.
- The existing Android workflow is reused. Upstream APKs, agent instructions,
  and previously removed desktop infrastructure are not imported.
- The three upstream renderer case studies are kept in [lessons](lessons/).
  Their useful content replaces the new entries in the removed cross-platform
  `PORTING_PATTERNS.md`; that archived document is not restored.

## Validation

Local checks completed:

- All 30 ordered DXVK patches apply to the pinned `46a3bc018bcae408d49d3c500e4e536a11f6789a`
  source; repeating the application logic confirms idempotency.
- The merged SDL patch applies to SDL `release-3.4.2`, commit
  `683181b47cfabd293e3ea409f838915b8297a4fd`.
- The actual native backend-selection functions compile with strict host warnings
  and pass 15 fresh-process configuration, default and environment cases.
- Android shell scripts, workflow YAML and its 17 embedded shell steps parse.
- Android resource XML parses; all 13 locales contain the render-picker strings.
- Bundled ANGLE libraries are AArch64 ELF shared objects with the expected SONAMEs.

Full Android compilation and APK packaging are validated through the existing
`build-android.yml` pull-request workflow. See the PR checks for the result.
Local helper checks alone do not establish that the full application compiles.

## Physical-device checks still required

- Upgrade v123 to v124 and confirm Vulkan is initially selected and renders.
- Launch each backend through the menu and into a skirmish; restart after changing detail.
- Check build icons, terrain, aircraft shadows, water, text, partial viewports and screen edges.
- Check Home/recents/resume, both landscape orientations, touch input and the GameSir controller.
- Check stock and nested game-data layouts, damaged archives and GeneralsOnline network failures.
- Compare the same map, units, detail and resolution across backends before claiming an FPS gain.

The upstream release's reported Mali FPS gain is not a measurement of this build
or of the OnePlus 15. CI is build validation, not device or gameplay validation.

## Attribution

Upstream integration credit belongs to MYSOREZ and the contributors named in its
commit history and source notices. The GLES backend also credits
[Lolendor/Generals-WebAssembly](https://github.com/Lolendor/Generals-WebAssembly).
ANGLE remains licensed by the ANGLE Project Authors; see its
[binary provenance](../../Core/Libraries/Source/d3d8gles/angle-prebuilt/README.md).
