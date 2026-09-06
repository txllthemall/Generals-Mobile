# Bundled ANGLE for Android ARM64

These two libraries are copied unchanged from
[MYSOREZ/GeneralsZH-Android-Port v1.2.0](https://github.com/MYSOREZ/GeneralsZH-Android-Port/tree/90b82c1ce59fbce03a0a47f1fccc1365b39eacf3/Core/Libraries/Source/d3d8gles/angle-prebuilt).
The upstream packaging notes identify an Android ARM64 Vulkan-only build from
the [AOSP ANGLE mirror](https://android.googlesource.com/platform/external/angle/).
That snapshot does not record the exact ANGLE source revision or a reproducible
build recipe; this integration does not claim to have rebuilt the libraries.

| File | SHA-256 |
|---|---|
| `arm64-v8a/libEGL_angle.so` | `68bec582dab9f2466ca83d80babbb65a334c09a9e85ef83aa3448ba22990268b` |
| `arm64-v8a/libGLESv2_angle.so` | `f85d6f551069a88757859088a88a3456fdfe4ac54e9c18a3c7fb533e9753efe2` |

Both are AArch64 ELF shared objects. Their distinct SONAMEs allow Android to
load them alongside the system GLES driver. Vulkan remains the default renderer;
the Setup picker explicitly selects ANGLE when wanted.

The [ANGLE license](LICENSE), obtained from the
[ANGLE project](https://github.com/google/angle/blob/main/LICENSE), is also
included in APK assets as `licenses/ANGLE-LICENSE.txt`.
