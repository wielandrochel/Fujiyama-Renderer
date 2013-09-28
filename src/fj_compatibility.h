/*
Copyright (c) 2011-2013 Hiroshi Tsubokawa
See LICENSE and README
*/

#ifndef COMPATIBILITY_H
#define COMPATIBILITY_H

#if defined(_WIN32)
  #define SI_WINDOWS
  #if defined(_WIN64)
    /* 64bit Windows */
    /*
    typedef char               int8_t;
    typedef int                int32_t;
    typedef unsigned int       uint32_t;
    typedef long               int64_t;
    typedef unsigned long long uintptr_t;
    #define UINT32_MAX UINT_MAX
    */
    #include <stdint.h>
  #else
    /* 32bit Windows */
    #include <stdint.h>
  #endif
#elif defined(__APPLE__) || defined(MACOSX)
    /* 64bit Mac OS X */
    #define SI_MACOSX
    typedef char               int8_t;
    typedef int                int32_t;
    typedef unsigned int       uint32_t;
    typedef long               int64_t;
    typedef unsigned long      uintptr_t;
    #define UINT32_MAX UINT_MAX
#else
  #define SI_LINUX
  #if defined(__x86_64__)
    /* 64bit Linux */
    /*
    typedef char               int8_t;
    typedef int                int32_t;
    typedef unsigned int       uint32_t;
    typedef long               int64_t;
    typedef unsigned long      uintptr_t;
    #define UINT32_MAX UINT_MAX
    */
    #include <stdint.h>
  #else
    /* 32bit Linux */
    /*
    typedef char               int8_t;
    typedef int                int32_t;
    typedef unsigned int       uint32_t
    __extension__
    typedef long long          int64_t;
    __extension__
    typedef unsigned long long uintptr_t;
    #define UINT32_MAX UINT_MAX
    */
    #include <stdint.h>
  #endif
#endif

#endif /* XXX_H */