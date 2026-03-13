# For testing Cross-library boundary safety of SingletonBase
add_subdirectory(externalSingleton)

find_package(Catch2 REQUIRED)

add_executable(tst_singletonbase
    tst_singletonbase.cpp
)

target_link_libraries(tst_singletonbase
    PRIVATE
        ExternalSingleton
        Catch2::Catch2WithMain
)

enable_testing()

add_test(NAME tst_singletonbase
    COMMAND tst_singletonbase
)
