cmake_minimum_required(VERSION 3.14)
project(sunny)

# GoogleTest requires at least C++17
# I have set it to 20 to use std::numbers::pi instead of defining my own constant
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(solar src/solar/sun_position.cpp)
target_include_directories(solar PUBLIC src)

add_executable(sunny src/app/main.cpp)
target_link_libraries(sunny PRIVATE solar)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.18.0.zip
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(solar_tests tests/sun_position_tests.cpp)
target_link_libraries(solar_tests PRIVATE solar GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(solar_tests)
