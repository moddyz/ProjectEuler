/// https://projecteuler.net/problem=3

#include <stdio.h>

#include <euler/profiler.h>
#include <euler/diagnostics.h>

bool isPrime(size_t number)
{
    PROFILE_FUNCTION();
    for (size_t i = 2; i < number; ++i) {
        if (number % i == 0) {
            return false;
        }
    }

    return true;
}

size_t ComputeLargestPrimeFactor(size_t number)
{
    PROFILE_FUNCTION();

    size_t maxPrimeFactor = 0;
    for (size_t i = 1; i <= number; ++i) {
        if (number % i == 0) {
            number = number / i;

            if (i > maxPrimeFactor && isPrime(i)) {
                maxPrimeFactor = i;
            }

            i = 1;
        }
    }

    return maxPrimeFactor;
}

int main()
{
    PROFILER_SETUP();

    size_t number = 600851475143;
    size_t maxPrimeFactor = ComputeLargestPrimeFactor(number);
    printf("The largest prime factor of %lu is %lu\n", number, maxPrimeFactor);
    ASSERT(maxPrimeFactor == 6857);

    PROFILER_PRINT();
    PROFILER_TEARDOWN();
}
