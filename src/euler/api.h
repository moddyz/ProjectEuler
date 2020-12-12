#pragma once

/// \file api.h
///
/// For providing storage-class information to the MSVC compiler.

#if defined(_WIN32) || defined(_WIN64)
#    if defined(euler_EXPORTS)
#        define EULER_API __declspec(dllexport)
#    else
#        define EULER_API __declspec(dllimport)
#    endif
#else
#    define EULER_API
#endif
