# Advanced Topics

This section covers advanced use cases, best practices, tips, and important considerations for using SingletonBase.

## Thread Safety

SingletonBase provides thread-safe access to singleton instances. The implementation uses appropriate synchronization mechanisms to ensure that:

- Instance creation is thread-safe
- Multiple threads can safely access the same singleton instance
- No race conditions occur during initialization

**Note:** While the singleton access is thread-safe, the singleton's own methods may need additional synchronization if they modify shared state.

## Lifetime Management

- **Initialization**: Singletons are created lazily on first access.
- **Destruction**: Singletons are destroyed at program exit.
- **Order**: Be aware of destruction order when singletons depend on each other.

## Testing Best Practices

### Using ScopedSingletonState

- Use `ScopedSingletonState` in unit tests to isolate singleton state.
- Ensure scoped states are properly scoped to avoid affecting other tests.
- Test both normal operation and edge cases with different instances.

### Isolation

1. Each test should start with a clean singleton state.
2. Use scoped states to prevent test interference.
3. Reset any singleton state modifications after each test.

### Production Code

2. Avoid using `ScopedSingletonState` in production code.
3. Singletons should be used judiciously in production.

### Destructor Order

3. Be aware of destructor order - scoped instances are destroyed before the `ScopedSingletonState` destructor runs.
4. Ensure proper cleanup of resources owned by scoped instances.

### Reference Lifetime

4. Don't store references to scoped instances beyond their lifetime.
5. Use the `instance()` method of `ScopedSingletonState` for temporary access.

## Cross-Library Boundaries

When using singletons across dynamic library boundaries:

- Use the `PM_SINGLETON_SAFE_*` macros instead of `PM_SINGLETON`.
- Ensure consistent declaration in headers and implementation in sources.
- Be aware of library loading/unloading order.

## Performance Considerations

- **Lazy Initialization**: Reduces startup time but may cause delays on first access.
- **Thread Safety Overhead**: Minimal overhead for subsequent accesses after initialization.
- **Memory Usage**: Singletons persist for the entire program lifetime.

## Common Pitfalls

- **Global State**: Singletons introduce global state, which can make testing and reasoning about code harder.
- **Tight Coupling**: Overuse of singletons can lead to tightly coupled code.
- **Initialization Order**: Problems can arise if singletons depend on each other during initialization.

## Alternatives

Consider alternatives to singletons when appropriate:
- Dependency injection
- Service locators
- Factory patterns
- Local instances with proper scoping

## Integration with Build Systems

### CMake

To integrate SingletonBase into your CMake project:

```cmake
# Add the SingletonBase directory
add_subdirectory(path/to/SingletonBase)

# Link against the interface library
target_link_libraries(your_target PM::SingletonBase)
```

### Building the Library

1. Ensure you have CMake installed on your system.
2. Clone the repository to your local machine.
3. Navigate to the root directory of the project.
4. Create a build directory:

   ```
   mkdir build
   cd build
   ```

5. Run CMake to configure the project:

   ```
   cmake ..
   ```

6. Build the project:

   ```
   cmake --build .
   ```

### Running Tests

After building the project, you can run the unit tests using:

```
ctest
```

### Other Build Systems

Since SingletonBase is header-only, simply include the headers in your include path and ensure the PM namespace is available.