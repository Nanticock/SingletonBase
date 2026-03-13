# Advanced Topics

This document covers advanced concepts, best practices, and implementation details for using SingletonBase effectively in complex scenarios.

## Table of Contents

<details>
<summary>Testing Best Practices</summary>
<ul>
<li><a href="#testing-best-practices">Testing Best Practices</a></li>
<li><a href="#using-scoped-singleton-state">Using ScopedSingletonState</a></li>
<li><a href="#test-isolation">Test Isolation</a></li>
<li><a href="#production-code">Production Code</a></li>
<li><a href="#reference-lifetime">Reference Lifetime</a></li>
<li><a href="#nested-scopes">Nested Scopes</a></li>
<li><a href="#filo-stack-behavior">FILO Stack Behavior</a></li>
<li><a href="#safe-cascading-behavior">Safe Cascading Behavior</a></li>
<li><a href="#best-practices-for-nested-scopes">Best Practices for Nested Scopes</a></li>
</ul>
</details>

<details>
<summary>Thread Safety</summary>
<ul>
<li><a href="#thread-safety">Thread Safety</a></li>
</ul>
</details>

<details>
<summary>Lifetime Management</summary>
<ul>
<li><a href="#lifetime-management">Lifetime Management</a></li>
<li><a href="#initialization">Initialization</a></li>
<li><a href="#destruction">Destruction</a></li>
<li><a href="#destruction-order">Destruction Order</a></li>
</ul>
</details>

<details>
<summary>Cross-Library Boundaries</summary>
<ul>
<li><a href="#cross-library-boundaries">Cross-Library Boundaries</a></li>
<li><a href="#library-loading-order">Library Loading Order</a></li>
</ul>
</details>

<details>
<summary>Performance Considerations</summary>
<ul>
<li><a href="#performance-considerations">Performance Considerations</a></li>
<li><a href="#zero-overhead-for-basic-singleton-usage">Zero Overhead for Basic Singleton Usage</a></li>
<li><a href="#minimal-cost-for-testing-capabilities">Minimal Cost for Testing Capabilities</a></li>
<li><a href="#scoped-state-implementation-details">Scoped State Implementation Details</a></li>
<li><a href="#performance-guarantees">Performance Guarantees</a></li>
</ul>
</details>

## Testing Best Practices

### Using ScopedSingletonState

`ScopedSingletonState` [see ScopedSingletonState class](classes/ScopedSingletonState.md) is the primary tool for making singletons testable. It provides RAII-based temporary instance replacement with automatic restoration.

```cpp
// Basic test isolation
void testMySingleton() {
    PM::ScopedSingletonState<MySingleton> scoped;
    // MySingleton::instance() now returns a fresh instance
    MySingleton::instance().setValue(42);
    assert(MySingleton::instance().getValue() == 42);
    // Original instance restored automatically
}
```

### Test Isolation

Proper test isolation is crucial for reliable unit testing:

1. **Clean State**: Each test should start with a predictable singleton state
2. **Scoped Replacement**: Use `ScopedSingletonState` to prevent test interference
3. **State Reset**: Ensure any modifications are properly reset after each test

```cpp
// Multiple tests with proper isolation
void testCase1() {
    PM::ScopedSingletonState<MySingleton> scoped;
    MySingleton::instance().configureForTest1();
    // Test logic...
}

void testCase2() {
    PM::ScopedSingletonState<MySingleton> scoped;
    MySingleton::instance().configureForTest2();
    // Different test logic...
}
```

### Production Code

While `ScopedSingletonState` is essential for testing, it should never be used in production code:

- **Production Safety**: Avoid using scoped states in release builds
- **Code Clarity**: Production code should use singletons directly without temporary replacements
- **Performance**: Scoped state creation has overhead that should be avoided in production

### Reference Lifetime

When working with scoped singleton instances:

- **Scope Awareness**: References to scoped instances become invalid when the scope ends
- **Temporary Access**: Use the `instance()` method of `ScopedSingletonState` for temporary access only
- **Avoid Storage**: Never store references to scoped instances beyond their intended lifetime

```cpp
// Correct usage
void testCorrect() {
    PM::ScopedSingletonState<MySingleton> scoped;
    auto& instance = scoped.instance(); // Direct access to scoped instance
    instance.doSomething();
    // instance becomes invalid here
}

// Incorrect usage - DON'T DO THIS
MySingleton* badPtr;
void testIncorrect() {
    PM::ScopedSingletonState<MySingleton> scoped;
    badPtr = &scoped.instance(); // Storing reference beyond scope
}
badPtr->doSomething(); // Undefined behavior!
```

> **Important:** Storing a reference to a scoped instance and accessing it after the `ScopedSingletonState` goes out of scope results in undefined behavior.

### Nested Scopes

`ScopedSingletonState` fully supports nested scopes with FILO (First In, Last Out) stack behavior. Creating nested scoped states is completely safe and the recommended pattern for complex testing scenarios.

#### FILO Stack Behavior

When you create nested `ScopedSingletonState` objects, they behave like a stack:

- **Innermost scope** has the highest priority and controls the singleton instance
- **Outer scopes** are temporarily suspended but maintain their state
- **Destruction** happens in reverse order (FILO), ensuring proper state restoration

```cpp
void testNestedScopes() {
    // Level 1: Original singleton instance
    MySingleton::instance().setValue(100);
    assert(MySingleton::instance().getValue() == 100);
    
    {
        // Level 2: First scoped state replaces instance
        PM::ScopedSingletonState<MySingleton> outerScope;
        MySingleton::instance().setValue(200);
        assert(MySingleton::instance().getValue() == 200);
        
        {
            // Level 3: Second scoped state replaces instance again
            PM::ScopedSingletonState<MySingleton> innerScope;
            MySingleton::instance().setValue(300);
            assert(MySingleton::instance().getValue() == 300);
            
            // innerScope destroyed here - restores Level 2 instance
        }
        
        // Back to Level 2 instance
        assert(MySingleton::instance().getValue() == 200);
        
        // outerScope destroyed here - restores Level 1 instance
    }
    
    // Back to original instance
    assert(MySingleton::instance().getValue() == 100);
}
```

#### Safe Cascading Behavior

The nested scope system provides robust safety guarantees:

- **Automatic State Preservation**: Each scope saves and restores the previous singleton state
- **Exception Safety**: Even if exceptions occur, all scopes will properly restore their states during stack unwinding
- **Memory Safety**: No memory leaks or dangling pointers, even with complex nesting
- **Performance**: Each nesting level adds minimal overhead (just pointer storage)
- **Predictability**: State restoration always happens in the correct reverse order

```cpp
void testExceptionSafety() {
    MySingleton::instance().setValue(100);
    
    try {
        PM::ScopedSingletonState<MySingleton> scope1;
        MySingleton::instance().setValue(200);
        
        {
            PM::ScopedSingletonState<MySingleton> scope2;
            MySingleton::instance().setValue(300);
            
            throw std::runtime_error("Test exception");
            // Even with exception, both scopes will restore properly
        }
    } catch (...) {
        // Original state is fully restored despite exception
        assert(MySingleton::instance().getValue() == 100);
    }
}
```

#### Best Practices for Nested Scopes

- **Use for Complex Scenarios**: Nested scopes are ideal for testing code that itself creates scoped states
- **Keep Nesting Shallow**: While safe, very deep nesting can become confusing to follow
- **Document Intent**: Use comments to clarify what each nesting level represents
- **Test Isolation**: Each level provides complete isolation from other levels

## Thread Safety

SingletonBase provides thread-safe access to singleton instances through the Meyers Singleton pattern implementation. The implementation uses appropriate synchronization mechanisms to ensure that:

- **Instance Creation**: Thread-safe initialization using static local variables with proper synchronization
- **Concurrent Access**: Multiple threads can safely access the same singleton instance simultaneously
- **Race Condition Prevention**: No race conditions occur during the initialization phase

```cpp
// Thread-safe singleton access
void threadFunction1() {
    MySingleton::instance().threadSafeMethod();
}

void threadFunction2() {
    MySingleton::instance().anotherThreadSafeMethod();
}

// Both threads can safely access the same instance
std::thread t1(threadFunction1);
std::thread t2(threadFunction2);
t1.join(); t2.join();
```

> **Note:** While the singleton access itself is thread-safe, the singleton's own methods may require additional synchronization if they modify shared state or perform non-atomic operations.

## Lifetime Management

Singletons created with SingletonBase follow lazy initialization - the instance is created only when first accessed through the `instance()` method. This approach:

- **Reduces Startup Time**: No singleton instances are created until they are actually needed
- **Avoids Unnecessary Memory Usage**: Resources are allocated only when required
- **Supports Complex Initialization**: Allows singletons to depend on other system components that may not be available at startup

```cpp
// Lazy initialization example
class ConfigManager : public PM::SingletonBase<ConfigManager> {
    PM_SINGLETON(ConfigManager)
public:
    std::string getConfig() {
        // Instance created here on first access
        return loadConfigFromFile();
    }
private:
    ConfigManager() {
        // Constructor called only when first accessed
        std::cout << "ConfigManager initialized" << std::endl;
    }
};
```

### Destruction

Singleton instances are automatically destroyed at program exit through the normal C++ static destruction mechanism. The destruction order follows the reverse order of initialization as defined by the C++ standard.

### Destruction Order

When singletons have dependencies on each other, destruction order becomes critical:

- **Reverse Initialization Order**: Singletons are destroyed in the reverse order of their creation
- **Dependency Management**: Ensure that singletons with dependencies are initialized in the correct order
- **Resource Cleanup**: All resources owned by the singleton should be properly released during destruction

```cpp
class Logger : public PM::SingletonBase<Logger> {
    PM_SINGLETON(Logger)
public:
    ~Logger() { std::cout << "Logger destroyed" << std::endl; }
};

class Database : public PM::SingletonBase<Database> {
    PM_SINGLETON(Database)
private:
    Database() {
        // Logger must be initialized first
        Logger::instance().log("Database initializing");
    }
public:
    ~Database() {
        Logger::instance().log("Database destroyed");
        // Logger still available during destruction
    }
};
```

> **Warning:** Circular dependencies between singletons can lead to undefined destruction behavior and should be avoided.

## Cross-Library Boundaries

When singletons need to be shared across dynamic library boundaries, special considerations apply:

- **Instance Sharing**: The same singleton instance must be accessible from multiple libraries
- **Memory Management**: Proper memory management across library boundaries
- **Loading Order**: Library loading and unloading order affects singleton availability

### Library Loading Order

The order in which dynamic libraries are loaded and unloaded directly impacts singleton behavior:

- **Initialization Dependencies**: Libraries with singleton dependencies must be loaded in the correct order
- **Cleanup Order**: Reverse loading order for proper cleanup
- **Runtime Library Management**: Be aware of how your application manages dynamic libraries

```cpp
// In header file (shared across libraries)
class SharedSingleton : public PM::SingletonBase<SharedSingleton> {
    PM_SINGLETON_SAFE_HEADER(SharedSingleton)
public:
    void sharedMethod();
};

// In exactly one source file
PM_SINGLETON_SAFE_SOURCE(SharedSingleton)

void SharedSingleton::sharedMethod() {
    // Implementation accessible from any library
}
```

> **Note:** Use the `PM_SINGLETON_SAFE_*` macros [see SingletonBase class](classes/SingletonBase.md) when creating singletons that will be used across library boundaries.

## Performance Considerations

SingletonBase follows a strict **"pay for what you use"** principle - you only incur costs for the features you actually use. The library itself adds zero overhead to your singleton's memory footprint or performance characteristics.

### Zero Overhead for Basic Singleton Usage

When you use SingletonBase for basic singleton functionality (without testing), you pay exactly the same as a traditional singleton:

- **Memory Cost**: Only the default singleton instance exists in memory
- **Performance**: Identical to any other singleton implementation
- **No Hidden Costs**: No additional data structures or indirections

```cpp
class MySingleton : public PM::SingletonBase<MySingleton> {
    PM_SINGLETON(MySingleton)
    // Your business logic here - no performance penalty
};
```

### Minimal Cost for Testing Capabilities

When you decide to use `ScopedSingletonState` [see ScopedSingletonState class](classes/ScopedSingletonState.md) for testing, you only pay for what you use:

- **Per-Test Instance**: Each `ScopedSingletonState` creates exactly one additional instance of your singleton
- **Stack Allocation**: If you create scoped states on the stack (recommended), you never pay for heap allocation
- **No VTable Overhead**: Instance replacement is done without any virtual function calls or vtable indirections
- **Automatic Cleanup**: All state management happens within the scoped object itself

```cpp
void testMySingleton() {
    // Only cost: one additional instance of MySingleton
    // No heap allocation if scoped is on stack
    PM::ScopedSingletonState<MySingleton> scoped;
    // Test your singleton with isolated state
}
```

### Scoped State Implementation Details

The `ScopedSingletonState` manages everything internally without affecting your singleton:

- **Member Variable Instance**: The new singleton instance is created as a member variable of `ScopedSingletonState`
- **Stack-Friendly**: Designed for stack allocation with zero heap overhead
- **Self-Contained**: All state restoration logic is encapsulated within the scoped object
- **Size Independence**: Your singleton's memory layout and size remain unchanged
- **No External Dependencies**: The scoped state manages instance pointers internally

```cpp
// Implementation concept (actual implementation may vary)
class ScopedSingletonState<T> {
    T scopedInstance;  // Member variable - no heap allocation needed
    T* previousInstance;  // Stored for restoration
    
public:
    ScopedSingletonState() {
        // Store current instance, replace with scoped instance
    }
    ~ScopedSingletonState() {
        // Restore previous instance
    }
};
```

### Performance Guarantees

- **No Runtime Overhead**: Basic singleton usage has identical performance to traditional implementations
- **Minimal Testing Overhead**: Testing only costs the memory of one additional instance per test scope
- **No Virtual Dispatch**: All instance management uses direct pointers, no vtable lookups
- **Predictable Costs**: Memory and performance costs scale linearly with testing usage
- **Zero Production Impact**: Testing code and overhead are completely absent in production builds

**Bottom Line**: SingletonBase gives you powerful testing capabilities with the confidence that you're only paying for what you actually use. Business logic singletons have zero overhead, and testing only costs the memory of additional instances during test execution.

**See also:** [Class Documentation](classes/)