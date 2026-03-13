#ifndef EXTERNALSINGLETON_H
#define EXTERNALSINGLETON_H

#include "ExternalSingleton_global.h"

#include <PM/SingletonBase.h>

#include <string>

class EXTERNAL_SINGLETON_API ExternalSingleton : public PM::SingletonBase<ExternalSingleton>
{
    PM_SINGLETON_SAFE_HEADER(ExternalSingleton)

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
