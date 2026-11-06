![CI](https://github.com/bllnr/sunny/actions/workflows/ci.yml/badge.svg)

This is Sunny! 
Sunny is a small program to calculate sun hours on building surfaces that I'm creating to learn C++.

# Notes to self

## How to set up a new project and dev environment
Install C/C++ extension for VSCode

Install MinGW
https://www.mingw-w64.org/

Install Cmake
https://cmake.org/download/

Install Ninja using pacman (in the MSYS2 UCRT64 terminal)
pacman -S mingw-w64-ucrt-x86_64-ninja

Install Doxygen
pacman -S mingw-w64-ucrt-x86_64-doxygen

To build for the first time (after this Cmake will Use Ninja)
cmake -S . -B build -G Ninja

## Build and test

To reconfigure before building:
cmake -S . -B build

To build:
cmake --build build

To run tests:
ctest --test-dir build --output-on-failure

Rebuild and test:
cmake --build build && ctest --test-dir build --output-on-failure