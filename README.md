# libnogo
Library for shared code between nogo server and client.

This library is built to be consumed by other cmake C projects. As such, the steps to add it to a cmake project will be described here.

## Add library

### Prerequisites
A C compiler with C99 support. The library has only been built and tested on Linux/MacOS. However, it does not use anything specific to those operating systems.

### Build and link
As stated earlier the library is meant to be consumed by other cmake C projects. Specifically by adding to projects with `add_subdirectory()`.
1. Download and place the folder inside of the project that will be using this library. Alternatively use something like `git submodule add` to copy the library into the project. These steps will assume it was placed in `lib/libnogo`.
2. In the main CMakeLists.txt add `add_subdirectory(lib/libnogo)`. Add the previous line before any other executable/libraries that will use libnogo.
3. After `add_executable()` or equivalent, link the library with `target_link_libraries(<target> PRIVATE libnogo)`. See cmake documentation for a better description of this function.
4. Include the header in any .c or .h file with `#include <libnogo/nogo.h>` and all symbols should be visible.

## Usage

### Options
Set these options before adding the subdirectory to control specific actions when compiling libnogo.
```
option(LIBNOGO_ENABLE_SANITIZERS "Compile with or without sanitizers" OFF)

option(LIBNOGO_ENABLE_TESTS "Determine if tests will be ran with root project" ON)
```
