# Build System Integration

This guide covers how to integrate SingletonBase into your project's build system and build the library from source.

## Table of Contents

<details>
<summary>CMake Integration</summary>
<ul>
<li><a href="#cmake-integration">CMake Integration</a></li>
</ul>
</details>

<details>
<summary>Building from Source</summary>
<ul>
<li><a href="#building-from-source">Building from Source</a></li>
<li><a href="#prerequisites">Prerequisites</a></li>
<li><a href="#build-steps">Build Steps</a></li>
<li><a href="#running-tests">Running Tests</a></li>
</ul>
</details>

<details>
<summary>Other Build Systems</summary>
<ul>
<li><a href="#other-build-systems">Other Build Systems</a></li>
</ul>
</details>

## CMake Integration

To integrate SingletonBase into your CMake-based project:

```cmake
# Add the SingletonBase directory to your project
add_subdirectory(path/to/SingletonBase)

# Link against the SingletonBase interface library
target_link_libraries(your_target PM::SingletonBase)
```

This approach ensures proper include paths and namespace availability.

## Building from Source

### Prerequisites

- **CMake**: Version 3.16 or higher must be installed on your system
- **C++ Compiler**: A C++17 compatible compiler (GCC 7+, Clang 5+, MSVC 2017+)
- **Git**: For cloning the repository

### Build Steps

1. **Clone Repository**:
   ```bash
   git clone https://github.com/your-org/SingletonBase.git
   cd SingletonBase
   ```

2. **Create Build Directory**:
   ```bash
   mkdir build
   cd build
   ```

3. **Configure with CMake**:
   ```bash
   cmake ..
   ```

4. **Build the Project**:
   ```bash
   cmake --build .
   ```

### Running Tests

After building the project, execute the test suite:

```bash
ctest
```

This runs all unit tests to verify the library functionality.

## Other Build Systems

Since SingletonBase is header-only, integration with other build systems is straightforward:

- **Include Path**: Add the SingletonBase include directory to your compiler's include path
- **Namespace Access**: Ensure the `PM` namespace is available in your compilation units
- **No Linking Required**: As a header-only library, no linking step is necessary

**See also:** [Advanced Topics](advanced-topics.md)