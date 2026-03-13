# ScopedSingletonState

`ScopedSingletonState<T>` is a RAII class that temporarily replaces the singleton instance of type T. This class is primarily designed for testing purposes, allowing isolated singleton state management without affecting other parts of the application.

## Table of Contents

<details>
<summary>Public Methods</summary>
<ul>
<li><a href="#scopedsingletonstate">ScopedSingletonState()</a></li>
<li><a href="#scopedsingletonstate-destructor">~ScopedSingletonState()</a></li>
<li><a href="#instance">instance()</a></li>
</ul>
</details>

<details>
<summary>Usage Example</summary>
<ul>
<li><a href="#scoped-state-example">Scoped State Example</a></li>
</ul>
</details>

<details>
<summary>Important Notes</summary>
<ul>
<li><a href="#lifetime-management">Lifetime Management</a></li>
<li><a href="#thread-safety">Thread Safety</a></li>
<li><a href="#copy-move-operations">Copy/Move Operations</a></li>
<li><a href="#testing-focus">Testing Focus</a></li>
<li><a href="#nested-scopes">Nested Scopes</a></li>
</ul>
</details>

<details>
<summary>Best Practices</summary>
<ul>
<li><a href="#testing-isolation">Testing Isolation</a></li>
<li><a href="#production-code">Production Code</a></li>
<li><a href="#destructor-order">Destructor Order</a></li>
<li><a href="#reference-lifetime">Reference Lifetime</a></li>
</ul>
</details>

## Public_Methods

### ScopedSingletonState()

Constructs a new scoped state, replacing the current singleton instance with a new one.

**Effects:**
- Creates a new instance of T
- Stores the current instance pointer
- Sets the singleton to point to the new instance

**See also:** [usage_example](#usage_example)

### ~ScopedSingletonState()

Destroys the scoped state, restoring the previous singleton instance.

**Effects:**
- Restores the singleton to point to the previously stored instance

**See also:** [usage_example](#usage_example)

### T &instance() const

Returns a reference to the scoped singleton instance.

**Returns:** Reference to the temporary singleton instance created by this scoped state.

**Note:** This allows direct access to the scoped instance for modification during testing.

**See also:** [usage_example](#usage_example)

## Usage_Example

### Scoped State Example

```cpp
#include <PM/ScopedSingletonState.h>
#include <cassert>

void testSingletonBehavior() {
    // Get original instance
    MySingleton *original = &MySingleton::instance();

    {
        // Create scoped state - instance is now different
        PM::internal::ScopedSingletonState<MySingleton> scopedState;

        assert(original != &MySingleton::instance());

        // Modify the scoped instance
        scopedState.instance().setValue(42);
        assert(MySingleton::instance().getValue() == 42);

        // Scoped state ends here
    }

    // Original instance is restored
    assert(original == &MySingleton::instance());
    assert(MySingleton::instance().getValue() != 42); // Assuming default value
}
```

**See also:** [public_methods](#public_methods)

## Important_Notes

### Lifetime Management

- **Lifetime Management:** The scoped instance is destroyed when the `ScopedSingletonState` object goes out of scope. Ensure that any resources owned by the scoped instance are properly cleaned up.

### Thread Safety

- **Thread Safety:** Inherits the thread safety characteristics of `SingletonBase`. Not thread-safe.

**See also:** [[SingletonBase#thread-safety]]

### Copy/Move Operations

- **Copy/Move Operations:** Explicitly deleted to prevent accidental copying or moving.

### Testing Focus

- **Testing Focus:** Primarily designed for unit testing to isolate singleton state between tests.

### Nested Scopes

- **Nested Scopes:** Multiple scoped states can be nested, with each level maintaining its own instance.

**See also:** [best_practices](#best_practices)

## Best_Practices

### Testing Isolation

1. Use in test functions to ensure test isolation

### Production Code

2. Avoid using in production code

### Destructor Order

3. Be aware of destructor order - the scoped instance is destroyed before the `ScopedSingletonState` destructor runs

### Reference Lifetime

4. Don't store references to the scoped instance beyond the scope's lifetime

**See also:** [important_notes](#important_notes)

## Dependencies

Requires `SingletonBase.h` for the underlying singleton mechanism.

**See also:** [[SingletonBase]]