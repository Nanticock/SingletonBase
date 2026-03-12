# SingletonBase Project

## Overview
SingletonBase is a C++ project that implements the Singleton design pattern. This project provides a robust and reusable Singleton implementation, along with unit tests to ensure its correctness.

## Project Structure
```
SingletonBase
├── CMakeLists.txt        # Root CMake configuration
├── src                   # Source files for the project
│   └── CMakeLists.txt    # CMake configuration for source files
├── tests                 # Unit tests for the project
│   ├── CMakeLists.txt    # CMake configuration for tests
│   ├── tst_singletonbase.cpp # Unit test implementation
│   └── externalSingleton  # External Singleton library
│       └── CMakeLists.txt # CMake configuration for external library
└── README.md             # Project documentation
```

## Build Instructions
1. Ensure you have CMake installed on your system.
2. Clone the repository to your local machine.
3. Navigate to the root directory of the project.
4. Create a build directory:
   ```
   mkdir build
   cd build
   ```
5. Run CMake to configure the project:
   ```
   cmake ..
   ```
6. Build the project:
   ```
   cmake --build .
   ```

## Running Tests
After building the project, you can run the unit tests using the following command:
```
ctest
```

## Usage
To use the SingletonBase library in your own projects, include the necessary headers and link against the compiled library. Refer to the documentation in the `src` directory for more details on the API and usage examples.