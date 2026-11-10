cmake_minimum_required(VERSION 3.20)
project(sunny)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ── Options ──────────────────────────────────────────────
option(SUNNY_BUILD_APP "Build the raylib GUI application" ON)

# ── Dependencies ─────────────────────────────────────────
include(FetchContent)

FetchContent_Declare(googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.18.0.zip
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

if(SUNNY_BUILD_APP)
  set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
  FetchContent_Declare(raylib
    URL https://github.com/raysan5/raylib/archive/refs/tags/6.2.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE)
  FetchContent_Declare(raygui
    URL https://github.com/raysan5/raygui/archive/refs/tags/5.0.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    SOURCE_SUBDIR does-not-exist)
  FetchContent_MakeAvailable(raylib raygui)
endif()

# ── Libraries (always built) ─────────────────────────────
add_library(core src/core/angles.cpp src/core/core.cpp)
target_include_directories(core PUBLIC src)

add_library(solar src/solar/sun_position.cpp)
target_link_libraries(solar PUBLIC core)

add_library(model src/model/model.cpp)
target_link_libraries(model PUBLIC core solar)

# ── GUI app (optional) ───────────────────────────────────
if(SUNNY_BUILD_APP)
  add_executable(sunny src/app/main.cpp)
  target_link_libraries(sunny PRIVATE model raylib)
  target_include_directories(sunny SYSTEM PRIVATE ${raygui_SOURCE_DIR}/src)
endif()

# ── Tests (always built) ─────────────────────────────────
enable_testing()
add_executable(unit_tests
  tests/sun_position_tests.cpp
  tests/model_tests.cpp
  tests/angles_tests.cpp)
target_link_libraries(unit_tests PRIVATE model GTest::gtest_main)
include(GoogleTest)
gtest_discover_tests(unit_tests)

# ── Warnings for our own targets ─────────────────────────
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  set(SUNNY_WARNINGS -Wall -Wextra -Wpedantic)
  foreach(t core solar model unit_tests)
    target_compile_options(${t} PRIVATE ${SUNNY_WARNINGS})
  endforeach()
  if(SUNNY_BUILD_APP)
    target_compile_options(sunny PRIVATE ${SUNNY_WARNINGS})
  endif()
endif()