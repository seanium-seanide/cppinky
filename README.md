# cppinky

A simple Pinky interpreter

## Build instructions

```
cmake --preset config-debug-clang
cmake --build --preset build-debug-clang
```

## Build and run unit tests

The following packages are required to build the project with tests:

* CMake >= 3.25
* Catch2 >= 3.15.3
* Clang++ >= 20

The following command can build and run all tests:

```
cmake --workflow --preset build-and-test-clang
```

## Usage

From project root directory:

```
./build/debug/clang/source/cppinky [<script filename>]
```
