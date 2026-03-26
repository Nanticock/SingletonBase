# SingletonBase

**Make your C++ singletons testable in 2 lines!**

Header-only C++11 library for testable singletons. Add one macro, get instant testability.

## Create a Testable Singleton

```cpp
#include <PM/SingletonBase.h>

class MySingleton : public PM::SingletonBase<MySingleton> {
    PM_SINGLETON(MySingleton)  // That's it!

public:
    void setValue(int v) { value_ = v; }
    int getValue() const { return value_; }

private:
    int value_ = 0;
};
```

## Write Unit Tests

```cpp
#include <PM/ScopedSingletonState.h>

void testMySingleton() {
    PM::ScopedSingletonState<MySingleton> scoped;  // Fresh instance

    MySingleton::instance().setValue(42);
    assert(MySingleton::instance().getValue() == 42);
}
```

No mocking. No setup. Just test directly.

##  Documentation

[docs](docs/README.md)

| Class | Description | Reference |
|-------|-------------|-----------|
| `SingletonBase<T>` | Testable singleton base | [API](docs/classes/SingletonBase.md) |
| `ScopedSingletonState<T>` | Test instance replacement | [API](docs/classes/ScopedSingletonState.md) |

Advanced topics: [Performance, Thread Safety, etc.](docs/advanced-topics.md)

## 🛠️ How to Add to Your Project

### Option 1: Manual (Simplest)
1. Copy the `include/` folder to your project
2. Add `include/` to your compiler's include paths
3. Include headers: `#include <PM/SingletonBase.h>`

### Option 2: CMake (Recommended)
Add this to your `CMakeLists.txt`:
```cmake
add_subdirectory(path/to/SingletonBase)
target_link_libraries(your_target PM::SingletonBase)
```

This automatically adds the include directory and excludes tests from your build.

## License

**MIT License** - Do whatever you want with it! No restrictions, no strings attached.
