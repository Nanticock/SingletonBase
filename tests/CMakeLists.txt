add_subdirectory(externalSingleton)

add_executable(tst_singletonbase
    tst_singletonbase.cpp
)

target_link_libraries(tst_singletonbase
    PRIVATE
        PM::Core
        ExternalSingleton
)

include(CTest)
enable_testing()

add_test(NAME tst_singletonbase
    COMMAND tst_singletonbase
)