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

## Build and test locally

cmake --preset dev
cmake --build --preset dev
ctest --preset dev

## Build and test (CI)

cmake --preset ci
cmake --build --preset ci
ctest --preset ci