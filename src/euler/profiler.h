#pragma once

#define _SCOPED_PROFILE(file, line, string)                                    \
    ScopedProfiler profile##line(file, line, string);
#define PROFILE(string) _SCOPED_PROFILE(__FILE__, __LINE__, string)
#define PROFILE_FUNCTION()                                                     \
    _SCOPED_PROFILE(__FILE__, __LINE__, __PRETTY_FUNCTION__)
#define PROFILER_SETUP() ProfilerSetup();
#define PROFILER_TEARDOWN() ProfilerTeardown();
#define PROFILER_PRINT() ProfilerPrint();

#include <euler/api.h>
#include <stdint.h>

/// Fwd declaration.
class ProfileRecord;

/// \class Profiler
///
/// Records the timing on Start() and Stop().
///
/// It also records metadata about the location in the source code and a user
/// supplied string.
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

    /// Record the ending time.  Publish both the starting and ending time.
    void Stop();

private:
    /// The profile record to author timings and metadata into.
    /// This memory is not owned by the Profiler instance itself, but by the
    /// internal global record store.
    ProfileRecord* m_profileRecord = nullptr;
};

/// \class ScopedProfiler
///
/// Records the timing with respect to the lifetime of this object.
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
