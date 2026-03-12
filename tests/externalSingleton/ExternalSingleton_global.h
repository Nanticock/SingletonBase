#ifndef EXTERNALSINGLETON_GLOBAL_H
#define EXTERNALSINGLETON_GLOBAL_H

#if defined(_WIN32) || defined(__CYGWIN__)
#    ifdef EXTERNALSINGLETON_LIB
#        define EXTERNAL_SINGLETON_API __declspec(dllexport)
#    else
#        define EXTERNAL_SINGLETON_API __declspec(dllimport)
#    endif
#else
#    if __GNUC__ >= 4
#        define EXTERNAL_SINGLETON_API __attribute__((visibility("default")))
#    else
#        define EXTERNAL_SINGLETON_API
#    endif
#endif

#endif // EXTERNALSINGLETON_GLOBAL_H
