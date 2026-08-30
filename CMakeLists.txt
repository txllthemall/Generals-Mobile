cmake_minimum_required(VERSION 3.25)

# Use packagename_ROOT for FindPackage.
if(POLICY CMP0074)
    cmake_policy(SET CMP0074 NEW)
endif()

# Disable default MSVC setting CRT type so we can set it ourselves.
if(POLICY CMP0091)
    cmake_policy(SET CMP0091 NEW)
endif()

# Disable default MSVC warning level so we can set it ourselves.
if(POLICY CMP0092)
    cmake_policy(SET CMP0092 NEW)
endif()

# Allow specifying MSVC debug configurations.
#if(POLICY CMP0141)
#    cmake_policy(SET CMP0141 NEW)
#endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

include(FeatureSummary)
include(CMakeDependentOption)

# We don't support in tree builds, so help people make the right choice.
if (CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR)
    message(FATAL_ERROR "Building in-source is not supported! Create a build dir and remove ${CMAKE_SOURCE_DIR}/CMakeCache.txt")
endif()

# Top level project, doesn't really affect anything.
project(genzh LANGUAGES C CXX)

# The Android build consumes the original engine's Win32-compatible API shim.
include_directories(${CMAKE_SOURCE_DIR}/Generals/Code/CompatLib/Include)

# Compiler cache (ccache) – must be included before compilers.cmake so the
# launcher is registered before CMake finalizes the compiler selection.
include(cmake/ccache.cmake)

# This file handles extra settings wanted/needed for different compilers.
include(cmake/compilers.cmake)

include(FetchContent)

# Build the Android DX8-to-Vulkan backend.
include(cmake/dx8.cmake)

# SDL3 provides Android windowing and input.
if(SAGE_USE_SDL3)
    include(cmake/sdl3.cmake)
endif()

# GeneralsX @feature Android port 10/07/2026 Optional custom Vulkan driver
# loading (Adreno/Turnip via libadrenotools) -- self-guarded with if(ANDROID).
include(cmake/adrenotools.cmake)

# The modern Android toolchain uses libc++; keep the legacy target name only as
# an interface dependency expected by shared engine CMake files.
add_library(stlport INTERFACE)

include(cmake/config.cmake)
include(cmake/gamespy.cmake)
include(cmake/lzhl.cmake)

# GeneralsX @bugfix fbraz 10/03/2026 Include openal.cmake here (before Core subdirectory)
# so OpenAL::OpenAL target is available when Core/GameEngineDevice/CMakeLists.txt processes.
# openal.cmake is self-guarded with if(SAGE_USE_OPENAL), so this is always safe to include.
include(cmake/openal.cmake)

# GeneralsX @feature BenderAI 21/04/2026 libcurl for in-game update checker (SAGE_UPDATE_CHECK builds)
# curl.cmake is self-guarded with if(SAGE_UPDATE_CHECK), so always safe to include.
include(cmake/curl.cmake)

# GeneralsX @feature fbraz 03/05/2026 Phase 4: Deterministic math library integration
# gamemath.cmake integrates fdlibm-based GameMath for cross-platform replay validation.
# Upstream reference: Okladnoj, PR #2670
# https://github.com/TheSuperHackers/GeneralsGameCode/pull/2670
# Self-guarded with if(SAGE_USE_DETERMINISTIC_MATH), so always safe to include.
include(cmake/gamemath.cmake)

add_subdirectory(Dependencies/Utility)

add_subdirectory(resources)

add_subdirectory(Core)

# GeneralsX @bugfix GitHubCopilot 14/04/2026 CompatLib must be configured on UNIX even when
# only Generals (not ZH) is built. It sets up d3d8lib INTERFACE includes for DXVK headers
# (windows_base.h) that core_wwcommon depends on transitively. When ZH is built,
# GeneralsMD/Code/CMakeLists.txt already includes CompatLib via add_subdirectory.
if(UNIX AND NOT RTS_BUILD_ZEROHOUR)
    add_subdirectory(GeneralsMD/Code/CompatLib)
endif()

# Add main build targets
if(RTS_BUILD_ZEROHOUR)
    add_subdirectory(GeneralsMD)
endif()

if(RTS_BUILD_GENERALS)
    add_subdirectory(Generals)
endif()

feature_summary(WHAT ENABLED_FEATURES DESCRIPTION "Enabled features:")
feature_summary(WHAT DISABLED_FEATURES DESCRIPTION "Disabled features:")
