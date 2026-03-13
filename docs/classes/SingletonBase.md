# SingletonBase

`PM::SingletonBase<T>` is a template class that provides singleton functionality for derived classes. It implements lazy initialization and supports cross-library boundary safety.

## Table of Contents

<details>
<summary>Macros</summary>
<ul>
<li><a href="#pm_singleton">PM_SINGLETON</a></li>
<li><a href="#pm_singleton_safe_header">PM_SINGLETON_SAFE_HEADER</a></li>
<li><a href="#pm_singleton_safe_source">PM_SINGLETON_SAFE_SOURCE</a></li>
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

## Macros

### PM_SINGLETON

Macro for declaring a singleton class.

**Parameters:**
- `CLASS_NAME`: The name of the singleton class.

**Usage:**
```cpp
class MySingleton : public PM::SingletonBase<MySingleton>
{
    PM_SINGLETON(MySingleton)
    // ...
};
```

### PM_SINGLETON_SAFE_HEADER

Macro for cross-library safe singleton declaration in header files.

**Parameters:**
- `CLASS_NAME`: The name of the singleton class.

**Usage:**
```cpp
class MySingleton : public PM::SingletonBase<MySingleton>
{
    PM_SINGLETON_SAFE_HEADER(MySingleton)
    // ...
};
```

### PM_SINGLETON_SAFE_SOURCE

Macro for cross-library safe singleton implementation in source files.

**Parameters:**
- `CLASS_NAME`: The name of the singleton class.

**Usage:**
In the source file (.cpp):
```cpp
PM_SINGLETON_SAFE_SOURCE(MySingleton)
```

## Public_Methods

### T &instance()

Returns the singleton instance.

**Returns:** Reference to the singleton instance.

**Thread Safety:** Thread-safe.

## Protected_Methods

### SingletonBase()

Protected constructor for the singleton base class.

### T **instanceRef_impl()

Protected static method for implementing instance reference logic.

**Returns:** Pointer to pointer to the instance.

**See also:** [Usage Examples](../usage-examples.md), [Advanced Topics](../advanced-topics.md)