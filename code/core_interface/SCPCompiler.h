
#pragma once

#include "scp_compiler_detection.h"

#define SCP_TOKEN_CONCAT1(x, y) x ## y
#define SCP_TOKEN_CONCAT(x, y) SCP_TOKEN_CONCAT1(x, y)

/**
 * Define for a function that should be constexpr but if the compiler doesn't support it then it uses inline
 */

#if SCP_COMPILER_CXX_CONSTEXPR
#define SCP_CONSTEXPR_FUNC constexpr
#else
#define SCP_CONSTEXPR_FUNC inline
#endif

#define SIZE_T_ARG    "%zu"
#define PTRDIFF_T_ARG "%zd"

#ifdef WIN32
#ifndef NDEBUG
#define UNREACHABLE(msg, ...)                                         \
	do {                                                              \
		GOutputDevice->Error(__FILE__, __LINE__, msg, ##__VA_ARGS__); \
	} while (false)
#else
#define UNREACHABLE(msg, ...) __assume(false)
#endif
#else
#ifndef NDEBUG
#define UNREACHABLE(msg, ...)                                         \
	do {                                                              \
		GOutputDevice->Error(__FILE__, __LINE__, msg, ##__VA_ARGS__); \
	} while (false)
#else
#define UNREACHABLE(msg, ...) __builtin_unreachable()
#endif

#endif

