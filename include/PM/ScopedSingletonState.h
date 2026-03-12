#ifndef SCOPEDSINGLETONSTATE_H
#define SCOPEDSINGLETONSTATE_H

#include "SingletonBase.h"

namespace PM
{
namespace internal
{
    template <typename T>
    class ScopedSingletonState
    {
    public:
        ScopedSingletonState() noexcept;
        ~ScopedSingletonState() noexcept;

        ScopedSingletonState(ScopedSingletonState<T> &&other) noexcept = delete;
        ScopedSingletonState(const ScopedSingletonState<T> &other) noexcept = delete;

        T &instance() const noexcept;

    private:
        T *m_lastInstance;
        mutable T m_instance;
    };
} // namespace internal
} // namespace PM

template <typename T>
inline PM::internal::ScopedSingletonState<T>::ScopedSingletonState() noexcept : m_lastInstance(nullptr)
{
    m_lastInstance = static_cast<T *>(T::setInstance(&m_instance));
}

template <typename T>
inline PM::internal::ScopedSingletonState<T>::~ScopedSingletonState() noexcept
{
    T::setInstance(m_lastInstance);
}

template <typename T>
inline T &PM::internal::ScopedSingletonState<T>::instance() const noexcept
{
    return m_instance;
}

#endif // SCOPEDSINGLETONSTATE_H
