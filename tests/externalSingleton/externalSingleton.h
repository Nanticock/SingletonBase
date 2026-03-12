#ifndef EXTERNALSINGLETON_H
#define EXTERNALSINGLETON_H

#include <Core/SingletonBase.h>
#include <Core/core_global.h>

#include <string>

#if defined(EXTERNAL_SINGLETON_LIBRARY)
#    define EXTERNAL_SINGLETON_API PM_DECL_EXPORT
#else
#    define EXTERNAL_SINGLETON_API PM_DECL_IMPORT
#endif

class EXTERNAL_SINGLETON_API ExternalSingleton : public PM::internal::SingletonBase<ExternalSingleton>
{
    PM_SINGLETON_BASE_SAFE_HEADER(ExternalSingleton)

public:
    ExternalSingleton();

    int stage() const;
    void setStage(int newStage);

    std::string stageName() const;
    void setStageName(const std::string &newStageName);

private:
    int m_stage;
    std::string m_stageName;
};

namespace TestNamespace
{
EXTERNAL_SINGLETON_API ExternalSingleton &getExternalSingletonInstance();
}

#endif // EXTERNALSINGLETON_H
