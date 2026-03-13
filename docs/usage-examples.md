# Usage Examples

This section provides practical examples of using SingletonBase and ScopedSingletonState.

## Basic Singleton Usage

To create a singleton class using SingletonBase:

```cpp
#include <PM/SingletonBase.h>

// Define your singleton class
class MySingleton : public PM::internal::SingletonBase<MySingleton>
{
    PM_SINGLETON_BASE(MySingleton)

public:
    void doSomething() {
        // Your singleton logic here
    }

private:
    MySingleton() {} // Private constructor
};

// Usage in code
int main() {
    MySingleton& instance = MySingleton::instance();
    instance.doSomething();
    return 0;
}
```

## Cross-Library Safe Singleton

For singletons that need to be shared across dynamic library boundaries:

```cpp
// In header file
class MySingleton : public PM::internal::SingletonBase<MySingleton>
{
    PM_SINGLETON_BASE_SAFE_HEADER(MySingleton)

public:
    void doSomething();

private:
    MySingleton();
};

// In source file
PM_SINGLETON_BASE_SAFE_SOURCE(MySingleton)

void MySingleton::doSomething() {
    // Implementation
}
```

## Scoped Singleton States for Testing

For testing purposes, you can use `ScopedSingletonState` to temporarily replace the singleton instance:

```cpp
#include <PM/ScopedSingletonState.h>

// In your test code
void testScopedState() {
    MySingleton* original = &MySingleton::instance();

    {
        PM::internal::ScopedSingletonState<MySingleton> scopedState;
        // Now MySingleton::instance() returns a different instance
        assert(original != &MySingleton::instance());

        // Modify the scoped instance
        scopedState.instance().setValue(42);
        assert(MySingleton::instance().getValue() == 42);
    }

    // Original instance is restored
    assert(original == &MySingleton::instance());
}
```