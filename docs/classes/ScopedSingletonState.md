# ScopedSingletonState

RAII class for temporarily replacing singleton instances during testing.

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
<li><a href="#scopedsingletonstate-1">ScopedSingletonState()</a></li>
<li><a href="#scopedsingletonstate-destructor">~ScopedSingletonState()</a></li>
<li><a href="#instance">instance()</a></li>
</ul>
</details>

## Overview

`PM::ScopedSingletonState<T>` is an RAII (Resource Acquisition Is Initialization) class that temporarily replaces the singleton instance of type `T` with a new instance. This class is specifically designed for testing purposes, enabling isolated singleton state management without affecting the global singleton state in other parts of the application.

The class works by storing the current singleton instance pointer upon construction and replacing it with a new instance. Upon destruction (when the scoped object goes out of scope), it automatically restores the original singleton instance. This ensures that test code can modify singleton state locally without side effects on other tests or the main application.

This RAII pattern guarantees that the singleton state is always properly restored, even if exceptions occur during testing. The class provides direct access to the temporary instance for modification during tests.

> **Note:** The template parameter `T` must be a type that publicly inherits from [SingletonBase](SingletonBase.md).

## Usage

### Basic Test Isolation

The most common usage is to add a `ScopedSingletonState` at the beginning of each test case function to ensure each test gets a fresh, unmodified singleton instance. This prevents test interference and ensures predictable test behavior.

```cpp
#include <PM/ScopedSingletonState.h>

void testSingleton() {
    // Create scoped state at test start for isolation
    PM::ScopedSingletonState<MySingleton> scoped;
    
    // MySingleton::instance() now returns a new, blank instance
    MySingleton::instance().setTestValue(42);
    
    // Test logic here...
    assert(MySingleton::instance().getValue() == 42);
}
// Original instance automatically restored when test ends
```

### Nested Scopes

For more granular control, create the scoped state within a nested scope to limit its effect to only a specific portion of the test code. This allows testing different singleton states within the same test function.

```cpp
void testWithNestedScopes() {
    MySingleton::instance().setValue(100); // Global instance
    
    {
        PM::ScopedSingletonState<MySingleton> innerScope;
        // Only this block sees the new instance
        assert(MySingleton::instance().getValue() != 100);
        innerScope.instance().setValue(200);
        assert(MySingleton::instance().getValue() == 200);
    }
    
    // Back to global instance
    assert(MySingleton::instance().getValue() == 100);
}
```

### Mocking with Derived Types

Create a `ScopedSingletonState` with a template parameter of a derived type from the original singleton class. This enables testing with mock implementations while maintaining the same interface.

```cpp
class MockSingleton : public MySingleton {
public:
    void setMockBehavior() { /* mock-specific setup */ }
};

void testWithMock() {
    PM::ScopedSingletonState<MockSingleton> mockScope;
    
    // MySingleton::instance() now returns the mock
    auto& mock = mockScope.instance();
    mock.setMockBehavior();
    
    // Test code interacts with mock through normal interface
    assert(MySingleton::instance().someMethod() == mockResult);
}
```

**See also:** [Advanced Topics](../advanced-topics.md) for detailed exploration of these usage patterns and best practices.

## Public_Methods

### ScopedSingletonState() noexcept

Constructs a new scoped state, replacing the current singleton instance with a new one.

**Effects:**
- Creates a new instance of T using the default constructor
- Stores a pointer to the current singleton instance for later restoration
- Replaces the singleton's instance pointer with the new instance

> **Note:** The new instance is created on the stack as a member or the scoped state object.

### ~ScopedSingletonState() noexcept

Destroys the scoped state, automatically restoring the previous singleton instance.

**Effects:**
- Restores the singleton to point to the previously stored instance
- Destroys the temporary instance created during construction

> **Note:** This destructor ensures proper cleanup and restoration, even if exceptions occur during the scoped block.

### T &instance() const noexcept

Returns a reference to the scoped singleton instance for direct access and modification during testing.

**Returns:** Reference to the temporary singleton instance created by this scoped state.

> **Note:** This method allows test code to modify the singleton's state locally without affecting the global singleton. Changes made through this reference are isolated to the current test scope.

**See also:** [Usage Examples](../usage-examples.md), [Advanced Topics](../advanced-topics.md)