/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/**
 * @file
 *
 * @brief Macros to abstract compiler capabilities (for doxygen use only)
 */

/**
 * @brief Identifies a printf-style format string
 */
#define SCP_FORMAT_STRING

/**
 * @brief Specify which arguments are involved in printf-style string formatting
 *
 * @details Expands to a compiler specific attribute which specify where the
 *          format arguments are located. Parameters are 1-based which also
 *          includes the 'this' parameter at position 1 for class methods.
 *
 * @param formatArg Location of format string argument in the argument list
 * @param varArgs Location where the variable arguments begin
 */
#define SCP_FORMAT_STRING_ARGS(formatArg,varArgs)

/**
 * @brief Format specifier for a @c size_t argument
 *
 * Due to different runtimes using different format specifier for these types
 * it's necessary to hide these changes behind a macro. Use this in place of %zu
 */
#define SIZE_T_ARG    "%zu"

/**
 * @brief Format specifier for a @c ptrdiff_t argument
 *
 * Due to different runtimes using different format specifier for these types
 * it's necessary to hide these changes behind a macro. Use this in place of %zd
 */
#define PTRDIFF_T_ARG "%zd"

/**
 * @brief Attribute for forcing a static variable to be instantiated
 *
 * This can be used to ensure that a static variable is present even if it isn't referenced in the translation unit
 */
#define USED_VARIABLE

/**
 * @brief For use in a case statement which falls through
 *
 * Some compilers issue a warning if a fallthrough is detected. This define can be used to suppress that warning.
 */
#define FALLTHROUGH

/**
 * @brief Specifies that an analyzer should consider this function as no-return
 */
#define CLANG_ANALYZER_NORETURN

/**
 * @brief Specifies that the code at which point this macro appears at is unreachable
 * @param msg The message to display in debug mode
 * @param ... Format arguments for the message
 */
#define UNREACHABLE(msg, ...)
