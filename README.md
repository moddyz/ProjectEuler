<a href="https://github.com/moddyz/ProjectEuler/actions?query=workflow%3A%22Build+and+test%22"><img src="https://github.com/moddyz/ProjectEuler/workflows/Build%20and%20test/badge.svg"/></a>

# ProjectEuler

C++ solutions to [Project Euler](https://projecteuler.net) problems.

## Table of Contents

- [Dependencies](#dependencies)
- [Building](#building)

### Dependencies

The following dependencies are mandatory:
- C++ compiler
- [CMake](https://cmake.org/documentation/) (3.12 or greater)

The following dependencies are optional:
- [Doxygen](https://www.doxygen.nl/index.html) and [graphviz](https://graphviz.org/) for documentation.

## Building

Example snippet for building this project:
```
mkdir build && cd build
cmake -DCMAKE_INSTALL_PREFIX="/apps/ProjectEuler/" ..
cmake --build  . -- VERBOSE=1 -j8 all test install
```
CMake options for configuring this project:

| CMake Variable name     | Description                                                            | Default |
| ----------------------- | ---------------------------------------------------------------------- | ------- |
| `BUILD_TESTING`         | Enable automated testing.                                              | `OFF`   |
