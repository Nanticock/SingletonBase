#ifndef SINGLETONBASE_H
#define SINGLETONBASE_H

// clang-format off
#define PM_SINGLETON_BASE(CLASS_NAME)                                \
    friend class PM::SingletonBase<CLASS_NAME>;                      \
    friend class PM::ScopedSingletonState<CLASS_NAME>;

#define PM_SINGLETON_BASE_SAFE_HEADER(CLASS_NAME)                    \
    PM_SINGLETON_BASE(CLASS_NAME)                                    \
    static CLASS_NAME **instanceRef_impl() noexcept;

#define PM_SINGLETON_BASE_SAFE_SOURCE(CLASS_NAME)                    \
    CLASS_NAME **CLASS_NAME::instanceRef_impl() noexcept             \
    {                                                                \
        return PM::SingletonBase<CLASS_NAME>::instanceRef_impl();    \
    }
// clang-format on

namespace PM
{
template <typename T>
class ScopedSingletonState;

template <typename T>
class SingletonBase
{
    template <typename Ty>
    friend class ScopedSingletonState;

public:
    static T &instance() noexcept;

protected:
    SingletonBase() noexcept = default;

    static T **instanceRef_impl() noexcept;

private:
    static T *setInstance(T *newInstance) noexcept;
    static T &defaultInstance() noexcept;
    static T **getInstanceRef() noexcept;

    SingletonBase(SingletonBase &&other) noexcept = delete;
    SingletonBase(const SingletonBase &other) noexcept = delete;

    SingletonBase &operator=(SingletonBase &&other) noexcept = delete;
    SingletonBase &operator=(const SingletonBase &other) noexcept = delete;
};
} // namespace PM

template <typename T>
inline T &PM::SingletonBase<T>::instance() noexcept
{
    return **getInstanceRef();
}

template <typename T>
inline T **PM::SingletonBase<T>::instanceRef_impl() noexcept
{
    static T *instancePtr = &defaultInstance();

    return &instancePtr;
}

template <typename T>
inline T *PM::SingletonBase<T>::setInstance(T *newInstance) noexcept
{
    T *oldInstance = *getInstanceRef();

    if (newInstance != nullptr)
        *getInstanceRef() = newInstance;
    else
        *getInstanceRef() = &defaultInstance();

    return oldInstance;
}

template <typename T>
inline T &PM::SingletonBase<T>::defaultInstance() noexcept
{
    static T result;

    return result;
}

template <typename T>
inline T **PM::SingletonBase<T>::getInstanceRef() noexcept
{
    return T::instanceRef_impl();
}

#endif // SINGLETONBASE_H
