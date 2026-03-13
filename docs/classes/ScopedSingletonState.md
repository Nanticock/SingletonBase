# ScopedSingletonState

`ScopedSingletonState<T>` is a RAII class that temporarily replaces the singleton instance of type T. This class is primarily designed for testing purposes, allowing isolated singleton state management without affecting other parts of the application.

## Table of Contents

<details>
<summary>Public Methods</summary>
<ul>
<li><a href="#scopedsingletonstate-1">ScopedSingletonState()</a></li>
<li><a href="#scopedsingletonstate-destructor">~ScopedSingletonState()</a></li>
<li><a href="#instance">instance()</a></li>
</ul>
</details>

## Public_Methods

### ScopedSingletonState()

Constructs a new scoped state, replacing the current singleton instance with a new one.

**Effects:**
- Creates a new instance of T
- Stores the current instance pointer
- Sets the singleton to point to the new instance

### ~ScopedSingletonState()

Destroys the scoped state, restoring the previous singleton instance.

**Effects:**
- Restores the singleton to point to the previously stored instance

### T &instance() const

Returns a reference to the scoped singleton instance.

**Returns:** Reference to the temporary singleton instance created by this scoped state.

> **Note:** This allows direct access to the scoped instance for modification during testing.

**See also:** [Usage Examples](../usage-examples.md), [Advanced Topics](../advanced-topics.md)