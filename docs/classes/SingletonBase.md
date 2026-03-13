# SingletonBase

CRTP base class that makes singletons testable by providing a unified, thread-safe instance() implementation.

## Table of Contents

<details>
<summary>Overview</summary>
<ul>
<li><a href="#overview">Overview</a></li>
<li><a href="#usage">Usage</a></li>
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
<li><a href="#singletonbase">SingletonBase()</a></li>
<li><a href="#instanceref_impl">instanceRef_impl()</a></li>
</ul>
</details>

<details>
<summary>Macros</summary>
<ul>
<li><a href="#pm_singleton">PM_SINGLETON</a></li>
<li><a href="#pm_singleton_safe_header">PM_SINGLETON_SAFE_HEADER</a></li>
<li><a href="#pm_singleton_safe_source">PM_SINGLETON_SAFE_SOURCE</a></li>
</ul>
</details>

## Overview

`PM::SingletonBase<T>` is a CRTP (Curiously Recurring Template Pattern) base class that serves as the backbone for making any singleton testable. It is designed specifically to preserve the API of already existing singletons, allowing you to retrofit an existing singleton by simply inheriting from `SingletonBase<T>` and adding the [PM_SINGLETON](#pm_singleton) macro. This transformation makes the singleton instantly testable without changing its external interface.

The class provides a unified implementation of the `instance()` function, which you cannot override. When retrofitting an existing singleton, you typically need to remove your custom `instance()` implementation and rely on the one provided by `SingletonBase`. This ensures that `instance()` is implemented consistently across all singletons in your project, following the Meyers Singleton pattern, which guarantees thread-safe access by default.

To use this CRTP class, you must pass your own class as the template parameter `T` and provide your class name to the macro for proper friend declarations and functionality.

By default, when you inherit from this class, your copy constructor and copy assignment operators become deleted as they are explicitly deleted in `SingletonBase`. The only way to access the constructor of your class is through the `instance()` function and [ScopedSingletonState](ScopedSingletonState.md) for your specific type.

### Usage

#### Creating a New Singleton

The simplest use case is creating a new singleton from scratch. Simply inherit from `SingletonBase<T>` and add the `PM_SINGLETON` macro as the first line in your class body:

```cpp
#include <PM/SingletonBase.h>

class MySingleton : public PM::SingletonBase<MySingleton>
{
    PM_SINGLETON(MySingleton)

public:
    void doSomething() {}

private:
    MySingleton() = default;
};

int main() {
    MySingleton& instance = MySingleton::instance();
    instance.doSomething();
}
```

#### Retrofitting an Existing Singleton

If you have an existing singleton with its own `instance()` implementation, you can retrofit it to use `SingletonBase` for testability. The process is similar, but you must remove your existing `instance()` method to ensure the `SingletonBase` implementation is used exclusively:

```cpp
// Before (existing singleton)
class ExistingSingleton
{
public:
    static ExistingSingleton& instance() {
        static ExistingSingleton inst;
        return inst;
    }

    void doSomething() {}

private:
    ExistingSingleton() = default;
};
```

```cpp
// After (retrofitted for testability)
#include <PM/SingletonBase.h>

class ExistingSingleton : public PM::SingletonBase<ExistingSingleton>
{
    PM_SINGLETON(ExistingSingleton)

public:
    void doSomething() {}

private:
    ExistingSingleton() = default;
    // Note: Removed the custom instance() method
};
```

By removing the custom `instance()` method, you ensure that all access goes through the unified, testable implementation provided by `SingletonBase`.

## Public_Methods

### T &instance()

Returns the singleton instance.

**Returns:** Reference to the singleton instance.

**Thread Safety:** Thread-safe.

## Protected_Methods

### SingletonBase()

Protected constructor for the singleton base class.

**Note:** This constructor is protected so that it can be accessed by any class inheriting from `SingletonBase` while preventing external access outside the singleton. This effectively makes any class that inherits from `SingletonBase` become a singleton automatically.

### T **instanceRef_impl()

Protected static method for implementing instance reference logic.

**Returns:** Pointer to pointer to the instance.

**Note:** This function is intended for internal use by the scoped singleton machinery only. Its implementation is either provided by the [PM_SINGLETON_SAFE_HEADER](#pm_singleton_safe_header) and [PM_SINGLETON_SAFE_SOURCE](#pm_singleton_safe_source) macros or the default `SingletonBase` implementation. You will never likely interact with this function directly.

## Macros

### PM_SINGLETON

The standard macro for declaring a singleton class. This macro adds the necessary friend declarations to allow `SingletonBase` and `ScopedSingletonState` to access private members of your class.

This macro must be added as the first line inside the class body of any class that directly inherits from `SingletonBase`. It is crucial for ensuring that the `SingletonBase::instance()` function works as expected.

**Parameters:**
- `CLASS_NAME`: The name of the singleton class.

**Important:** If this macro is not added, you will encounter compilation errors indicating that `defaultInstance` is trying to access an inaccessible member of your class. If you see such errors, it means you forgot to add this macro to your class.

**Usage:**
```cpp
class MySingleton : public PM::SingletonBase<MySingleton>
{
    PM_SINGLETON(MySingleton)  // Must be the first line in the class body

public:
    void doSomething() {}

private:
    MySingleton() = default;
};
```

### PM_SINGLETON_SAFE_HEADER

Macro for cross-library safe singleton declaration in header files. Use this when your singleton needs to be shared across dynamic library boundaries. This macro includes the standard `PM_SINGLETON` declarations plus additional setup for cross-library safety.
This macro has to be used in tandem with the [PM_SINGLETON_SAFE_SOURCE](#pm_singleton_safe_source) macro.

**Parameters:**
- `CLASS_NAME`: The name of the singleton class.

> **Important:** When using this macro, you do not need to use the `PM_SINGLETON` macro as it is already included. However, you must add the `PM_SINGLETON_SAFE_SOURCE` macro in one of your source files (.cpp) for the class. If you forget this, you will encounter compilation errors indicating that `instanceRef_impl` cannot be found in any of the object files. If you see such errors, it means you forgot to add `PM_SINGLETON_SAFE_SOURCE` for this specific class.

**Usage:**
```cpp
class MySingleton : public PM::SingletonBase<MySingleton>
{
    PM_SINGLETON_SAFE_HEADER(MySingleton)

public:
    void doSomething();

private:
    MySingleton();
};
```

**See also:** [Cross-library safety](../advanced-topics.md#cross-library-safety)

### PM_SINGLETON_SAFE_SOURCE

Macro for cross-library safe singleton implementation in source files. This must be paired with `PM_SINGLETON_SAFE_HEADER` in the header file and provides the necessary implementation to ensure proper instance sharing across dynamic library boundaries.

**Parameters:**
- `CLASS_NAME`: The name of the singleton class.

**Important:** This macro must be placed in exactly one source file (.cpp) for each class that uses `PM_SINGLETON_SAFE_HEADER`. It implements the `instanceRef_impl` function required for cross-library singleton sharing. If this macro is not added or is added multiple times, you will encounter linker errors.

**Usage:**
In the source file (.cpp):
```cpp
#include "MySingleton.h"  // Include the header

PM_SINGLETON_SAFE_SOURCE(MySingleton)

void MySingleton::doSomething() {
    // Implementation
}
```

**See also:** [Usage Examples](../usage-examples.md), [Advanced Topics](../advanced-topics.md)