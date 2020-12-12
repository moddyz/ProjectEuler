/// https://projecteuler.net/problem=1

#include <stdio.h>

#include <euler/profiler.h>
#include <euler/diagnostics.h>

int SumOfMultiplesOfThreeOrFive(int upperLimit)
{
    PROFILE_FUNCTION();
    int sum = 0;
    for (int i = 0; i < upperLimit; ++i) {
        if (i % 3 == 0 || i % 5 == 0) {
            sum += i;
        }
    }

    return sum;
}

int main()
{
    PROFILER_SETUP();

    int upperLimit = 1000;
    int sum = SumOfMultiplesOfThreeOrFive(upperLimit);
    printf(
        "The sum of numbers which are a multiple of 3 and 5 under %i is: %i\n",
        upperLimit,
        sum);
    ASSERT(sum == 233168);

    PROFILER_PRINT();
    PROFILER_TEARDOWN();
}
