add_library(ExternalSingleton SHARED
    externalSingleton.h
    ExternalSingleton.cpp
    ExternalSingleton_global.h
)

target_link_libraries(ExternalSingleton
    PUBLIC
        SingletonBase
)

target_include_directories(ExternalSingleton
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_definitions(ExternalSingleton
    PRIVATE
        EXTERNALSINGLETON_LIB
)
