#include "externalSingleton.h"

PM_SINGLETON_BASE_SAFE_SOURCE(ExternalSingleton)

ExternalSingleton::ExternalSingleton() : m_stage(0), m_stageName("initial stage")
{
}

int ExternalSingleton::stage() const
{
    return m_stage;
}

void ExternalSingleton::setStage(int newStage)
{
    m_stage = newStage;
}

std::string ExternalSingleton::stageName() const
{
    return m_stageName;
}

void ExternalSingleton::setStageName(const std::string &newStageName)
{
    m_stageName = newStageName;
}

ExternalSingleton &TestNamespace::getExternalSingletonInstance()
{
    return ExternalSingleton::instance();
}
