#include "profiler.h"

#include <algorithm>
#include <atomic>
#include <cstring>
#include <mutex>
#include <sstream>
#include <stdio.h>
#include <vector>

// Compute the duration between \ref i_start and \ref i_stop.
static timespec _ComputeDuration(const timespec& i_start,
                                 const timespec& i_stop)
{
    timespec duration;
    if ((i_stop.tv_nsec - i_start.tv_nsec) < 0) {
        duration.tv_sec = i_stop.tv_sec - i_start.tv_sec - 1;
        duration.tv_nsec = i_stop.tv_nsec - i_start.tv_nsec + 1e9;
    } else {
        duration.tv_sec = i_stop.tv_sec - i_start.tv_sec;
        duration.tv_nsec = i_stop.tv_nsec - i_start.tv_nsec;
    }
    return duration;
}

// Number of bytes to allocated for per-record string storage.
constexpr uint32_t c_profileRecordStringCapacity = 88;

/// A single profile record.
struct alignas(128) ProfileRecord
{
    char m_string[c_profileRecordStringCapacity]; // 88 bytes
    timespec m_start = { 0, 0 };                  // 104 bytes
    timespec m_stop = { 0, 0 };                   // 120 bytes
    uint32_t m_line = 0;                          // 124 bytes
    uint32_t m_stack = 0;                         // 128 bytes

    void Print() const
    {
        timespec duration = _ComputeDuration(m_start, m_stop);
        uint32_t microseconds = duration.tv_sec * 1e6 + duration.tv_nsec / 1e3;

        std::stringstream ss;
        for (uint32_t i = 0; i < m_stack; ++i) {
            ss << " ";
        }
        ss << "\\_";

        printf("=== Profiler Timings ===\n");
        printf("%s User-string: '%s', line: %u, stack: %u, duration: %u us\n",
               ss.str().c_str(),
               m_string,
               m_line,
               m_stack,
               microseconds);
    }
};

static_assert(sizeof(ProfileRecord) == 128);

/// Singleton store of profile records.
class ProfileRecordStore final
{
public:
    ProfileRecordStore(uint32_t i_recordCapacity)
    {
        m_records.resize(i_recordCapacity);
    }

    ~ProfileRecordStore() = default;

    /// Cannot copy.
    ProfileRecordStore(const ProfileRecordStore&) = delete;
    ProfileRecordStore& operator=(const ProfileRecordStore&) = delete;

    /// Check out the next record to author timing and metadata into.
    ProfileRecord* Checkout()
    {
        int index = m_recordIndex.fetch_add(1, std::memory_order_relaxed);
        if (index >= m_records.size()) {
            m_recordsLoopAround = true;

            // Only one of the threads should be resetting the index back to 0,
            // unless enough threads accumulate and loop around twice!
            if (index == m_records.size()) {
                m_recordIndex.store(0);
            }

            // Make sure we don't step out of bounds for all profiles which
            // enter this if clause.
            index = index % m_records.size();
        }

        return &m_records[index];
    }

    /// Return a reference to the records.
    const std::vector<ProfileRecord>& GetRecords() const { return m_records; }

    /// Get the size of the records.
    uint32_t GetRecordsSize() const
    {
        if (m_recordsLoopAround.load()) {
            // If we have reached capacity at some point, then all records are
            // valid.
            return m_records.size();
        } else {
            int index = m_recordIndex.load();
            // There is a chance that we reach capacity between the:
            // m_recordsLoopAround.load()
            // and
            // m_recordIndex.load()
            // So make sure we return a size value.
            return index < m_records.size() ? index : m_records.size();
        }
    }

private:
    /// Current record index.
    std::atomic_int m_recordIndex{ 0 };

    /// Have each record been authored at least once?
    std::atomic_bool m_recordsLoopAround{ false };

    /// Allocated records.
    std::vector<ProfileRecord> m_records;
};

/// Singleton pointer.
static ProfileRecordStore* g_recordStore = nullptr;

/// Global mutex to guard setup and teardown of store.
static std::mutex g_profileRecordStoreMutex;

void ProfilerSetup(uint32_t i_capacity)
{
    const std::lock_guard<std::mutex> lock(g_profileRecordStoreMutex);
    if (g_recordStore == nullptr) {
        g_recordStore = new ProfileRecordStore(i_capacity);
    }
}

void ProfilerTeardown()
{
    const std::lock_guard<std::mutex> lock(g_profileRecordStoreMutex);
    if (g_recordStore != nullptr) {
        delete g_recordStore;
        g_recordStore = nullptr;
    }
}

void ProfilerPrint()
{
    const std::lock_guard<std::mutex> lock(g_profileRecordStoreMutex);
    if (g_recordStore == nullptr) {
        return;
    }

    uint32_t recordsSize = g_recordStore->GetRecordsSize();
    std::vector<ProfileRecord> records = g_recordStore->GetRecords();
    std::sort(records.begin(),
              records.begin() + recordsSize,
              [](const ProfileRecord& a, const ProfileRecord& b) {
                  if (a.m_start.tv_sec == b.m_start.tv_sec) {
                      return a.m_start.tv_nsec < b.m_start.tv_nsec;
                  } else {
                      return a.m_start.tv_sec < b.m_start.tv_sec;
                  }
              });

    for (uint32_t recordIndex = 0; recordIndex < recordsSize; ++recordIndex) {
        records[recordIndex].Print();
    }
}

Profiler::Profiler(const char* i_file, uint32_t i_line, const char* i_string)
{
    if (g_recordStore != nullptr) {
        m_profileRecord = g_recordStore->Checkout();
        m_profileRecord->m_line = i_line;
        strncpy(
            m_profileRecord->m_string, i_string, c_profileRecordStringCapacity);
    }
}

/// Thread-local global for tracking stack position of this profile.
static thread_local uint32_t c_profileStack = 0;

void Profiler::Start()
{
    if (m_profileRecord != nullptr) {
        m_profileRecord->m_stack = c_profileStack++;
        clock_gettime(CLOCK_MONOTONIC, &(m_profileRecord->m_start));
    }
}

void Profiler::Stop()
{
    if (m_profileRecord != nullptr) {
        clock_gettime(CLOCK_MONOTONIC, &(m_profileRecord->m_stop));
        c_profileStack--;
    }
}

ScopedProfiler::ScopedProfiler(const char* i_file,
                               uint32_t i_line,
                               const char* i_string)
  : Profiler(i_file, i_line, i_string)
{
    Start();
}

ScopedProfiler::~ScopedProfiler()
{
    Stop();
}
