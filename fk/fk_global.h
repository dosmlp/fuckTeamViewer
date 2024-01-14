#ifndef FK_GLOBAL_H
#define FK_GLOBAL_H

#if defined(FK_LIBRARY)
#  define FKSHARED_EXPORT __declspec(dllexport)
#else
#  define FKSHARED_EXPORT __declspec(dllimport)
#endif

#endif // FK_GLOBAL_H
