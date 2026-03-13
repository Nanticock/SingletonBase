# Advanced Topics

This document covers advanced concepts, best practices, and implementation details for using SingletonBase effectively in complex scenarios.

## Table of Contents

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
<summary>Testing Best Practices</summary>
<ul>
<li><a href="#testing-best-practices">Testing Best Practices</a></li>
<li><a href="#using-scoped-singleton-state">Using ScopedSingletonState</a></li>
<li><a href="#test-isolation">Test Isolation</a></li>
<li><a href="#production-code">Production Code</a></li>
<li><a href="#reference-lifetime">Reference Lifetime</a></li>
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
<li><a href="#lazy-initialization">Lazy Initialization</a></li>
<li><a href="#thread-safety-overhead">Thread Safety Overhead</a></li>
<li><a href="#memory-usage">Memory Usage</a></li>
</ul>
</details>

<details>
<summary>Common Pitfalls</summary>
<ul>
<li><a href="#common-pitfalls">Common Pitfalls</a></li>
<li><a href="#global-state">Global State</a></li>
<li><a href="#tight-coupling">Tight Coupling</a></li>
<li><a href="#initialization-order-problems">Initialization Order Problems</a></li>
</ul>
</details>

<details>
<summary>Alternatives</summary>
<ul>
<li><a href="#alternatives">Alternatives</a></li>
</ul>
</details>


## Thread Safety

SingletonBase provides thread-safe access to singleton instances through the Meyers Singleton pattern implementation. The implementation uses appropriate synchronization mechanisms to ensure that:

- **Instance Creation**: Thread-safe initialization using static local variables with proper synchronization
- **Concurrent Access**: Multiple threads can safely access the same singleton instance simultaneously
- **Race Condition Prevention**: No race conditions occur during the initialization phase

> **Note:** While the singleton access itself is thread-safe, the singleton's own methods may require additional synchronization if they modify shared state or perform non-atomic operations.

## Lifetime Management

### Initialization

Singletons created with SingletonBase follow lazy initialization - the instance is created only when first accessed through the `instance()` method. This approach:

- **Reduces Startup Time**: No singleton instances are created until they are actually needed
- **Avoids Unnecessary Memory Usage**: Resources are allocated only when required
- **Supports Complex Initialization**: Allows singletons to depend on other system components that may not be available at startup

### Destruction

Singleton instances are automatically destroyed at program exit through the normal C++ static destruction mechanism. The destruction order follows the reverse order of initialization as defined by the C++ standard.

### Destruction Order

When singletons have dependencies on each other, destruction order becomes critical:

- **Reverse Initialization Order**: Singletons are destroyed in the reverse order of their creation
- **Dependency Management**: Ensure that singletons with dependencies are initialized in the correct order
- **Resource Cleanup**: All resources owned by the singleton should be properly released during destruction

> **Warning:** Circular dependencies between singletons can lead to undefined destruction behavior and should be avoided.

## Testing Best Practices

### Using ScopedSingletonState

`ScopedSingletonState` is the primary tool for making singletons testable. It provides RAII-based temporary instance replacement with automatic restoration.

### Test Isolation

Proper test isolation is crucial for reliable unit testing:

1. **Clean State**: Each test should start with a predictable singleton state
2. **Scoped Replacement**: Use `ScopedSingletonState` to prevent test interference
3. **State Reset**: Ensure any modifications are properly reset after each test

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

> **Important:** Storing a reference to a scoped instance and accessing it after the `ScopedSingletonState` goes out of scope results in undefined behavior.

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

> **Note:** Use the `PM_SINGLETON_SAFE_*` macros when creating singletons that will be used across library boundaries.

## Performance Considerations

### Lazy Initialization

The lazy initialization approach provides several performance benefits:

- **Startup Performance**: Reduces application startup time by deferring singleton creation
- **Memory Efficiency**: Only allocates memory when the singleton is actually needed
- **First Access Latency**: May introduce a small delay on first access due to initialization

### Thread Safety Overhead

While thread safety is essential, it comes with some overhead:

- **Synchronization Cost**: Initial access may involve synchronization primitives
- **Subsequent Access**: Minimal overhead after initialization is complete
- **Lock Contention**: Multiple threads accessing uninitialized singletons may experience contention

### Memory Usage

Singleton memory usage patterns:

- **Persistent Allocation**: Singletons remain allocated for the entire program lifetime
- **Static Storage**: Uses static storage duration, managed by the runtime
- **Resource Management**: Singletons should properly manage any resources they own

## Common Pitfalls

### Global State

Singletons introduce global state, which can complicate code reasoning and testing:

- **Hidden Dependencies**: Global state creates implicit dependencies between components
- **Test Complexity**: Global state makes unit testing more challenging
- **State Corruption**: Accidental modifications can affect unrelated parts of the system

### Tight Coupling

Overuse of singletons can lead to tightly coupled code:

- **Dependency Injection**: Consider using dependency injection instead of singletons
- **Interface Segregation**: Use interfaces to reduce coupling
- **Modular Design**: Design components to be more independent

### Initialization Order Problems

Initialization order issues can cause subtle bugs:

- **Static Initialization Order Fiasco**: Avoid depending on initialization order between translation units
- **Complex Dependencies**: Be cautious with singletons that depend on other singletons
- **Testing Challenges**: Initialization order issues may not manifest in simple test scenarios

## Alternatives

Consider these alternatives when singletons may not be the best solution:

- **Dependency Injection**: Pass dependencies explicitly rather than accessing global instances
- **Service Locators**: Use service locator patterns for more flexible dependency resolution
- **Factory Patterns**: Create objects through factories when singleton semantics aren't required
- **Local Instances**: Use properly scoped local instances when global state isn't needed

**See also:** [Class Documentation](classes/)