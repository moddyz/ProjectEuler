#pragma once

/// \file diagnostics.h
///
/// Diagnostic utilities.

#include <cassert>

/// \def ASSERT
///
/// When NDEBUG is \em not defined, abort the application if
/// the expression passed into ASSERT(...) does not evaluate to \p true.
#define ASSERT(expr) assert(expr);
