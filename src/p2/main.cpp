/// https://projecteuler.net/problem=2

#include <stdio.h>
#include <vector>
#include <numeric>

#include <euler/profiler.h>
#include <euler/diagnostics.h>

/// Generate an array sequence of fibonacci numbers whose value
/// is less than \p upperLimit.
///
/// \param upperLimit Upper limit of the term value.
///
/// \return Array of fibonacci numbers.
std::vector<uint32_t> GenerateFibonacci(uint32_t upperLimit)
{
    std::vector<uint32_t> fibArray = { 1, 2 };

    uint32_t firstTerm = 1;
    uint32_t secondTerm = 2;
    uint32_t nextTerm = firstTerm + secondTerm;
    while (nextTerm < upperLimit) {
        fibArray.push_back(nextTerm);
        firstTerm = secondTerm;
        secondTerm = nextTerm;
        nextTerm = firstTerm + secondTerm;
    }

    return fibArray;
}

/// Simple sum operator for even terms.
///
/// \param accum The current accumulation value
/// \param term The current term.
///
/// \return The next accumulation value.
uint32_t SumOfEvenNumbers(uint32_t accum, uint32_t term)
{
    if ((term & 1) == 0) {
        return accum + term;
    }
    else
    {
        return accum;
    }
}

uint32_t SumOfFibonacciUnder(uint32_t upperLimit)
{
    PROFILE_FUNCTION();
    std::vector<uint32_t> fibArray = GenerateFibonacci(upperLimit);
    return std::accumulate(
        fibArray.begin(), fibArray.end(), 0, SumOfEvenNumbers);
}

int main()
{
    PROFILER_SETUP();

    uint32_t accum = SumOfFibonacciUnder(4000000);
    printf("%u\n", accum);

    PROFILER_PRINT();
    PROFILER_TEARDOWN();
}
