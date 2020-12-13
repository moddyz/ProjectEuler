#pragma once

/// \file profiler.h
///
/// Simple instrumenting profiler.
///
/// A complete example usage:
/// \code{.cpp}
/// #include <euler/profiler.h>
///
/// int doWork() {
///     PROFILE_FUNCTION();
///     int sum = 0;
///     for (int i = 0; i < 100000; ++i ) {
///         sum += i;
///     }
///     return sum;
/// }
///
/// int main() {
///     PROFILER_SETUP();
///
///     doWork();
///
///     PROFILER_PRINT();
///     PROFILER_TEARDOWN();
/// }
/// \endcode

#include <euler/api.h>
#include <stdint.h>

/// \def PROFILER_SETUP
///
/// Allocate the memory required for profiling.
#define PROFILER_SETUP() ProfilerSetup();

#define _SCOPED_PROFILE(file, line, string)                                    \
    ScopedProfiler profile##line(file, line, string);

/// \def PROFILE
///
/// Insert a scoped profiler tagged with a user-supplied string.
#define PROFILE(string) _SCOPED_PROFILE(__FILE__, __LINE__, string)

/// \def PROFILE_FUNCTION
///
/// Insert a scoped profiler tagged with the pretty-function interpretation
/// of the parent function.
#define PROFILE_FUNCTION()                                                     \
    _SCOPED_PROFILE(__FILE__, __LINE__, __PRETTY_FUNCTION__)

/// \def PROFILER_TEARDOWN
///
/// Free all the memory allocated for profiling.
#define PROFILER_TEARDOWN() ProfilerTeardown();

/// \def PROFILER_PRINT
///
/// Pretty-print all the profiled timings in a human-readable form.
#define PROFILER_PRINT() ProfilerPrint();

/// Fwd declaration.
class ProfileRecord;

/// \class Profiler
///
/// Records the timing on Start() and Stop().
///
/// It also records call site information and a user supplied string.
class EULER_API Profiler
{
public:
    explicit Profiler(const char* i_file,
                      uint32_t i_line,
                      const char* i_string);
    ~Profiler() = default;

    // Cannot be copied.
    Profiler(const Profiler& i_profile) = delete;
    Profiler& operator=(const Profiler& i_profile) = delete;

    /// Record the starting time.
    void Start();

    /// Record the ending time.
    void Stop();

private:
    /// The profile record to author timings and metadata into.
    /// This memory is not owned by the Profiler instance itself, but by the
    /// internal global record store.
    ProfileRecord* m_profileRecord = nullptr;
};

/// \class ScopedProfiler
///
/// Similar to \ref Profiler, but records the timing with respect to the
/// lifetime of an instance of this class.
class EULER_API ScopedProfiler final : public Profiler
{
public:
    explicit ScopedProfiler(const char* i_file,
                            uint32_t i_line,
                            const char* i_string);
    ~ScopedProfiler();

    // Cannot be copied.
    ScopedProfiler(const ScopedProfiler& i_profile) = delete;
    ScopedProfiler& operator=(const ScopedProfiler& i_profile) = delete;
};

/// Allocate memory used for profiling.
///
/// \param i_capacity total number of records to allocate.  If the number of
/// profile instances exceed this number, it will loop back to the initial
/// record (oldest records will begin to be overwritten).
EULER_API
void ProfilerSetup(uint32_t i_capacity = 10000);

/// Deallocate memory used for profiling.
EULER_API
void ProfilerTeardown();

/// Print all profiled records.
EULER_API
void ProfilerPrint();
