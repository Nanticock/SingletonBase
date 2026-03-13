# ScopedSingletonState

RAII class for temporarily replacing singleton instances during testing.

## Table of Contents

<details>
<summary>Overview</summary>
<ul>
<li><a href="#overview">Overview</a></li>
</ul>
</details>

<details>
<summary>Public Methods</summary>
<ul>
<li><a href="#scopedsingletonstate-1">ScopedSingletonState()</a></li>
<li><a href="#scopedsingletonstate-destructor">~ScopedSingletonState()</a></li>
<li><a href="#instance">instance()</a></li>
</ul>
</details>

## Overview

`PM::ScopedSingletonState<T>` is a RAII class that temporarily replaces the singleton instance of type T. This class is primarily designed for testing purposes, allowing isolated singleton state management without affecting other parts of the application.

### Usage Example

```cpp
#include <PM/ScopedSingletonState.h>

void testSingleton() {
    auto* original = &MySingleton::instance();

    {
        PM::ScopedSingletonState<MySingleton> scoped;
        // MySingleton::instance() now returns a different instance
        assert(&MySingleton::instance() != original);
    }

    // Original instance restored
    assert(&MySingleton::instance() == original);
}
```

## Public_Methods

### ScopedSingletonState() noexcept

Constructs a new scoped state, replacing the current singleton instance with a new one.

**Effects:**
- Creates a new instance of T
- Stores the current instance pointer
- Sets the singleton to point to the new instance

### ~ScopedSingletonState() noexcept

Destroys the scoped state, restoring the previous singleton instance.

**Effects:**
- Restores the singleton to point to the previously stored instance

### T &instance() const noexcept

Returns a reference to the scoped singleton instance.

**Returns:** Reference to the temporary singleton instance created by this scoped state.

**Note:** This allows direct access to the scoped instance for modification during testing.

**See also:** [Usage Examples](../usage-examples.md), [Advanced Topics](../advanced-topics.md)