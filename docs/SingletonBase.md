# SingletonBase

`SingletonBase<T>` is a template class that provides singleton functionality for derived classes. It implements lazy initialization and supports cross-library boundary safety.

## Table of Contents

<details>
<summary>Macros</summary>
<ul>
<li><a href="#pm_singleton_base">PM_SINGLETON_BASE</a></li>
<li><a href="#pm_singleton_base_safe_header">PM_SINGLETON_BASE_SAFE_HEADER</a></li>
<li><a href="#pm_singleton_base_safe_source">PM_SINGLETON_BASE_SAFE_SOURCE</a></li>
</ul>
</details>

<details>
<summary>Public Methods</summary>
<ul>
<li><a href="#instance">instance()</a></li>
</ul>
</details>

<details>
<summary>Protected Methods</summary>
<ul>
<li><a href="#singletonbase-1">SingletonBase()</a></li>
<li><a href="#instanceref_impl">instanceRef_impl()</a></li>
</ul>
</details>

<details>
<summary>Usage Examples</summary>
<ul>
<li><a href="#basic-singleton">Basic Singleton</a></li>
<li><a href="#cross-library-safe-singleton">Cross-Library Safe Singleton</a></li>
</ul>
</details>

<details>
<summary>Thread Safety</summary>
<ul>
<li><a href="#thread-safety-notes">Thread Safety Notes</a></li>
</ul>
</details>

<details>
<summary>Related Classes</summary>
<ul>
<li><a href="#scopedsingletonstate">ScopedSingletonState</a></li>
</ul>
</details>

## Macros

### PM_SINGLETON_BASE

Declares friendship with the singleton base classes for the specified class.

**Parameters:**
- `CLASS_NAME`: The name of the class that will be a singleton.

**See also:** [usage_examples](#usage_examples)

### PM_SINGLETON_BASE_SAFE_HEADER

Extends `PM_SINGLETON_BASE` and adds a declaration for `instanceRef_impl()` to support cross-library singleton safety.

**Parameters:**
- `CLASS_NAME`: The name of the class.

**See also:** [Cross-Library Safe Singleton](#cross-library-safe-singleton)

### PM_SINGLETON_BASE_SAFE_SOURCE

Provides the implementation for `instanceRef_impl()` in the source file.

**Parameters:**
- `CLASS_NAME`: The name of the class.

**See also:** [Cross-Library Safe Singleton](#cross-library-safe-singleton)

## Public_Methods

### instance()

Returns a reference to the singleton instance.

**Returns:** Reference to the singleton instance.

**Thread Safety:** Not thread-safe.

**See also:** [thread_safety](#thread_safety)

## Protected_Methods

### SingletonBase()

Default constructor. Protected to prevent direct instantiation.

### instanceRef_impl()

Returns a pointer to the instance pointer. Must be implemented by derived classes when using cross-library safety.

**Returns:** Pointer to the instance pointer.

**See also:** [Cross-Library Safe Singleton](#cross-library-safe-singleton)

## Usage_Examples

### Basic Singleton

```cpp
#include <PM/SingletonBase.h>

class Logger : public PM::internal::SingletonBase<Logger>
{
    PM_SINGLETON_BASE(Logger)

public:
    void log(const std::string &message) {
        // Implementation
    }

private:
    Logger() = default;
};

// Usage
Logger::instance().log("Hello World");
```

### Cross-Library Safe Singleton

**Header (MySingleton.h):**
```cpp
#include <PM/SingletonBase.h>

class MySingleton : public PM::internal::SingletonBase<MySingleton>
{
    PM_SINGLETON_BASE_SAFE_HEADER(MySingleton)

public:
    void doSomething();

private:
    MySingleton() = default;
};
```

**Source (MySingleton.cpp):**
```cpp
#include "MySingleton.h"

PM_SINGLETON_BASE_SAFE_SOURCE(MySingleton)

void MySingleton::doSomething() {
    // Implementation
}
```

## Thread_Safety

### Thread Safety Notes

**See also:** [public_methods](#public_methods)

## Related_Classes

### ScopedSingletonState

For testing purposes, `ScopedSingletonState` can temporarily replace the singleton instance.

**See also:** [ScopedSingletonState](ScopedSingletonState.md)