#ifndef SINGLETONBASE_H
#define SINGLETONBASE_H

// clang-format off
#define PM_SINGLETON_BASE(CLASS_NAME)                                          \
    friend class PM::internal::SingletonBase<CLASS_NAME>;                      \
    friend class PM::internal::ScopedSingletonState<CLASS_NAME>;

#define PM_SINGLETON_BASE_SAFE_HEADER(CLASS_NAME)                              \
    PM_SINGLETON_BASE(CLASS_NAME)                                              \
    static CLASS_NAME **instanceRef_impl() noexcept;

#define PM_SINGLETON_BASE_SAFE_SOURCE(CLASS_NAME)                              \
    CLASS_NAME **CLASS_NAME::instanceRef_impl() noexcept                       \
    {                                                                          \
        return PM::internal::SingletonBase<CLASS_NAME>::instanceRef_impl();    \
    }
// clang-format on

namespace PM
{
namespace internal
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
} // namespace internal
} // namespace PM

template <typename T>
inline T &PM::internal::SingletonBase<T>::instance() noexcept
{
    return **getInstanceRef();
}

template <typename T>
inline T **PM::internal::SingletonBase<T>::instanceRef_impl() noexcept
{
    static T *instancePtr = &defaultInstance();

    return &instancePtr;
}

template <typename T>
inline T *PM::internal::SingletonBase<T>::setInstance(T *newInstance) noexcept
{
    T *oldInstance = *getInstanceRef();

    if (newInstance != nullptr)
        *getInstanceRef() = newInstance;
    else
        *getInstanceRef() = &defaultInstance();

    return oldInstance;
}

template <typename T>
inline T &PM::internal::SingletonBase<T>::defaultInstance() noexcept
{
    static T result;

    return result;
}

template <typename T>
inline T **PM::internal::SingletonBase<T>::getInstanceRef() noexcept
{
    return T::instanceRef_impl();
}

#endif // SINGLETONBASE_H
