#include <ExternalSingleton.h>

#include <PM/ScopedSingletonState.h>
#include <PM/SingletonBase.h>

#include <catch2/catch_test_macros.hpp>

#include <string>

namespace
{
const char FILE_SYSTEM_NAME[] = "FileSystem";
const char MOC_FILE_SYSTEM_NAME[] = "Moc file system";
} // namespace

class Communication : public PM::SingletonBase<Communication>
{
    PM_SINGLETON_BASE(Communication)

protected:
    inline Communication()
    {
    }
};

class FileSystem : public PM::SingletonBase<FileSystem>
{
    PM_SINGLETON_BASE(FileSystem)

public:
    virtual std::string name() const
    {
        return FILE_SYSTEM_NAME;
    }

protected:
    inline FileSystem()
    {
    }
};

class FileSystemMoc : public FileSystem
{
public:
    std::string name() const override
    {
        return m_name;
    }

    void setName(const std::string &value)
    {
        m_name = value;
    }

    inline FileSystemMoc() : m_name(MOC_FILE_SYSTEM_NAME)
    {
    }

private:
    std::string m_name;
};

TEST_CASE("instanceConsistency")
{
    REQUIRE(&FileSystem::instance() == &FileSystem::instance());
    REQUIRE(&Communication::instance() == &Communication::instance());

    void *fileSystemInstance = &FileSystem::instance();
    void *communicationInstance = &Communication::instance();
    REQUIRE(fileSystemInstance != communicationInstance);
}

TEST_CASE("scopedState")
{
    Communication *oldCommunicationInstance = &Communication::instance();

    {
        PM::ScopedSingletonState<Communication> communicationScopedState;
        REQUIRE(oldCommunicationInstance != &Communication::instance());

        REQUIRE(&Communication::instance() == &Communication::instance());
        REQUIRE(&communicationScopedState.instance() == &Communication::instance());
        REQUIRE(&communicationScopedState.instance() == &communicationScopedState.instance());
    }

    REQUIRE(oldCommunicationInstance == &Communication::instance());
}

TEST_CASE("nestedScopedStates")
{
    Communication *state0 = &Communication::instance();

    // state1
    {
        PM::ScopedSingletonState<Communication> state1;
        REQUIRE(&state1.instance() == &Communication::instance());

        REQUIRE(state0 != &state1.instance());

        // state2
        {
            PM::ScopedSingletonState<Communication> state2;
            REQUIRE(&state2.instance() == &Communication::instance());

            REQUIRE(state0 != &state1.instance());
            REQUIRE(&state1.instance() != &state2.instance());
        }
        REQUIRE(&state1.instance() == &Communication::instance());
    }

    REQUIRE(state0 == &Communication::instance());
}

TEST_CASE("polymorphicObjects")
{
    REQUIRE(FileSystem::instance().name() == FILE_SYSTEM_NAME);

    {
        PM::ScopedSingletonState<FileSystemMoc> mocState;

        REQUIRE(FileSystem::instance().name() == MOC_FILE_SYSTEM_NAME);

        const char newName[] = "NewName";
        mocState.instance().setName(newName);

        REQUIRE(FileSystem::instance().name() == newName);
    }
    REQUIRE(FileSystem::instance().name() == FILE_SYSTEM_NAME);
}

TEST_CASE("BUG83_crossLibraryBoundarySafety")
{
    REQUIRE(&ExternalSingleton::instance() == &TestNamespace::getExternalSingletonInstance());
}

TEST_CASE("BUG83_crossLibraryBoundaryScopedStateSafety")
{
    PM::ScopedSingletonState<ExternalSingleton> baseState;
    REQUIRE(&ExternalSingleton::instance() == &baseState.instance());
    REQUIRE(&TestNamespace::getExternalSingletonInstance() == &baseState.instance());

    const int newStageValue = 2;
    ExternalSingleton::instance().setStage(newStageValue);
    REQUIRE(ExternalSingleton::instance().stage() == newStageValue);
    REQUIRE(ExternalSingleton::instance().stage() == TestNamespace::getExternalSingletonInstance().stage());

    const char newStageName[] = "new stage";
    ExternalSingleton::instance().setStageName(newStageName);
    REQUIRE(ExternalSingleton::instance().stageName() == newStageName);
    REQUIRE(ExternalSingleton::instance().stageName() == TestNamespace::getExternalSingletonInstance().stageName());
}

TEST_CASE("BUG83_crossLibraryBoundaryNestedScopedStateSafety")
{
    PM::ScopedSingletonState<ExternalSingleton> baseState;

    const std::string oldStageName = ExternalSingleton::instance().stageName();
    {
        PM::ScopedSingletonState<ExternalSingleton> newState;

        const char newStageName[] = "stage 1";
        newState.instance().setStageName(newStageName);

        REQUIRE(newState.instance().stageName() == newStageName);
        REQUIRE(ExternalSingleton::instance().stageName() == newStageName);
        REQUIRE(TestNamespace::getExternalSingletonInstance().stageName() == newStageName);
    }
    REQUIRE(oldStageName == ExternalSingleton::instance().stageName());
    REQUIRE(oldStageName == TestNamespace::getExternalSingletonInstance().stageName());
}
