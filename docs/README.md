## Overview

SingletonBase is a C++ header-only library that implements the Singleton design pattern. This project provides a robust and reusable Singleton implementation, along with unit tests to ensure its correctness. It supports scoped singleton states for testing and cross-library boundary safety.

## Project Structure

```
SingletonBase
├── CMakeLists.txt        # Root CMake configuration
├── include/              # Header files
│   └── PM/               # PM namespace headers
│       ├── SingletonBase.h
│       └── ScopedSingletonState.h
├── docs/                 # Documentation
│   ├── README.md         # Project documentation
│   ├── SingletonBase.md  # SingletonBase.h documentation
│   └── ScopedSingletonState.md # ScopedSingletonState.h documentation
├── tests/                # Unit tests
│   ├── CMakeLists.txt    # CMake configuration for tests
│   ├── tst_singletonbase.cpp # Unit test implementation
│   └── externalSingleton/ # External Singleton library for cross-library testing
│       ├── CMakeLists.txt
│       ├── externalSingleton.cpp
│       └── externalSingleton.h
├── LICENSE
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

## Documentation

Detailed API documentation for the header files is available in the `docs/` directory:

- [SingletonBase](SingletonBase.md) - Documentation for `SingletonBase.h`
- [ScopedSingletonState](ScopedSingletonState.md) - Documentation for `ScopedSingletonState.h`

## Usage

To use the SingletonBase library in your own projects, include the necessary headers and link against the `PM::SingletonBase` interface library.

### Typical Use Case: Creating a Singleton Class

Here's how to create a singleton class using SingletonBase:

```cpp
#include <PM/SingletonBase.h>

// Define your singleton class
class MySingleton : public PM::internal::SingletonBase<MySingleton>
{
    PM_SINGLETON_BASE(MySingleton)

public:
    void doSomething() {
        // Your singleton logic here
    }

private:
    MySingleton() {} // Private constructor
};

// Usage in code
int main() {
    MySingleton& instance = MySingleton::instance();
    instance.doSomething();
    return 0;
}
```

### Scoped Singleton States for Testing

For testing purposes, you can use `ScopedSingletonState` to temporarily replace the singleton instance:

```cpp
#include <PM/ScopedSingletonState.h>

// In your test code
void testScopedState() {
    MySingleton* original = &MySingleton::instance();

    {
        PM::internal::ScopedSingletonState<MySingleton> scopedState;
        // Now MySingleton::instance() returns a different instance
        assert(original != &MySingleton::instance());
    }

    // After scope, original instance is restored
    assert(original == &MySingleton::instance());
}
```

## Testing

The project includes unit tests that verify the correctness of the Singleton implementation, including instance consistency, scoped states, polymorphism, and cross-library boundary safety.

### Running Tests

After building the project, you can run the unit tests using the following command:

```
ctest
```

### Test Structure

The tests are implemented as plain C++ functions that return `true` on success and `false` on failure. The main function calls all test functions and reports the results.

Example test function:

```cpp
bool testInstanceConsistency() {
    if (&MySingleton::instance() != &MySingleton::instance()) {
        return false; // Test failed
    }
    return true; // Test passed
}
```

        // Now MySingleton::instance() returns a different instance

        assert(original != &MySingleton::instance());

    }

  

    // After scope, original instance is restored

    assert(original == &MySingleton::instance());

}

```

  

## Testing

The project includes unit tests that verify the correctness of the Singleton implementation, including instance consistency, scoped states, polymorphism, and cross-library boundary safety.

  

### Running Tests

After building the project, you can run the unit tests using the following command:

```

ctest

```

### Test Structure

The tests are implemented as plain C++ functions that return `true` on success and `false` on failure. The main function calls all test functions and reports the results.

  

Example test function:

```cpp

bool testInstanceConsistency() {

    if (&MySingleton::instance() != &MySingleton::instance()) {

        return false; // Test failed

    }

    return true; // Test passed

}

```