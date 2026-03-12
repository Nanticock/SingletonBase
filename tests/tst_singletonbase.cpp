#include <ExternalSingleton.h>

#include <PM/SingletonBase.h>
#include <PM/ScopedSingletonState.h>

#include <string>

namespace
{
const char FILE_SYSTEM_NAME[] = "FileSystem";
const char MOC_FILE_SYSTEM_NAME[] = "Moc file system";
} // namespace

class Communication : public PM::internal::SingletonBase<Communication>
{
    PM_SINGLETON_BASE(Communication)

public:
    //

protected:
    inline Communication()
    {
    }
};

class FileSystem : public PM::internal::SingletonBase<FileSystem>
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

class tst_SingletonBase : public QObject
{
    Q_OBJECT

private slots:
    void instanceConsistency();
    void scopedState();
    void nestedScopedStates();
    void polymorphicObjects();

    void BUG83_crossLibraryBoundarySafety();
    void BUG83_crossLibraryBoundaryScopedStateSafety();
    void BUG83_crossLibraryBoundaryNestedScopedStateSafety();
};

void tst_SingletonBase::instanceConsistency()
{
    QCOMPARE(&FileSystem::instance(), &FileSystem::instance());
    QCOMPARE(&Communication::instance(), &Communication::instance());

    void *fileSystemInstance = &FileSystem::instance();
    void *communicationInstance = &Communication::instance();
    QVERIFY(fileSystemInstance != communicationInstance);
}

void tst_SingletonBase::scopedState()
{
    Communication *oldCommunicationInstance = &Communication::instance();

    {
        PM::internal::ScopedSingletonState<Communication> communicationScopedState;
        QVERIFY(oldCommunicationInstance != &Communication::instance());

        QCOMPARE(&Communication::instance(), &Communication::instance());
        QCOMPARE(&communicationScopedState.instance(), &Communication::instance());
        QCOMPARE(&communicationScopedState.instance(), &communicationScopedState.instance());
    }

    QCOMPARE(oldCommunicationInstance, &Communication::instance());
}

void tst_SingletonBase::nestedScopedStates()
{
    Communication *state0 = &Communication::instance();

    // state1
    {
        PM::internal::ScopedSingletonState<Communication> state1;
        QCOMPARE(&state1.instance(), &Communication::instance());

        QVERIFY(state0 != &state1.instance());

        // state2
        {
            PM::internal::ScopedSingletonState<Communication> state2;
            QCOMPARE(&state2.instance(), &Communication::instance());

            QVERIFY(state0 != &state1.instance());
            QVERIFY(&state1.instance() != &state2.instance());
        }
        QCOMPARE(&state1.instance(), &Communication::instance());
    }

    QCOMPARE(state0, &Communication::instance());
}

void tst_SingletonBase::polymorphicObjects()
{
    QCOMPARE(FileSystem::instance().name(), FILE_SYSTEM_NAME);

    {
        PM::internal::ScopedSingletonState<FileSystemMoc> mocState;

        QCOMPARE(FileSystem::instance().name(), MOC_FILE_SYSTEM_NAME);

        const char newName[] = "NewName";
        mocState.instance().setName(newName);

        QCOMPARE(FileSystem::instance().name(), newName);
    }
    QCOMPARE(FileSystem::instance().name(), FILE_SYSTEM_NAME);
}

void tst_SingletonBase::BUG83_crossLibraryBoundarySafety()
{
    QCOMPARE(&ExternalSingleton::instance(), &TestNamespace::getExternalSingletonInstance());
}

void tst_SingletonBase::BUG83_crossLibraryBoundaryScopedStateSafety()
{
    PM::internal::ScopedSingletonState<ExternalSingleton> baseState;
    QCOMPARE(&ExternalSingleton::instance(), &baseState.instance());
    QCOMPARE(&TestNamespace::getExternalSingletonInstance(), &baseState.instance());

    const int newStageValue = 2;
    ExternalSingleton::instance().setStage(newStageValue);
    QCOMPARE(ExternalSingleton::instance().stage(), newStageValue);
    QCOMPARE(ExternalSingleton::instance().stage(), TestNamespace::getExternalSingletonInstance().stage());

    const char newStageName[] = "new stage";
    ExternalSingleton::instance().setStageName(newStageName);
    QCOMPARE(ExternalSingleton::instance().stageName(), newStageName);
    QCOMPARE(ExternalSingleton::instance().stageName(), TestNamespace::getExternalSingletonInstance().stageName());
}

void tst_SingletonBase::BUG83_crossLibraryBoundaryNestedScopedStateSafety()
{
    PM::internal::ScopedSingletonState<ExternalSingleton> baseState;

    const std::string oldStageName = ExternalSingleton::instance().stageName();
    {
        PM::internal::ScopedSingletonState<ExternalSingleton> newState;

        const char newStageName[] = "stage 1";
        newState.instance().setStageName(newStageName);

        QCOMPARE(newState.instance().stageName(), newStageName);
        QCOMPARE(ExternalSingleton::instance().stageName(), newStageName);
        QCOMPARE(TestNamespace::getExternalSingletonInstance().stageName(), newStageName);
    }
    QCOMPARE(oldStageName, ExternalSingleton::instance().stageName());
    QCOMPARE(oldStageName, TestNamespace::getExternalSingletonInstance().stageName());
}

QTEST_MAIN(tst_SingletonBase)
#include "tst_singletonbase.moc"
