/// https://projecteuler.net/problem=1

#include <stdio.h>
#include <cassert>

int SumOfMultiplesOfThreeOrFive(int upperLimit)
{
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
    int upperLimit = 1000;
    int sum = SumOfMultiplesOfThreeOrFive(upperLimit);
    printf(
        "The sum of numbers which are a multiple of 3 and 5 under %i is: %i\n",
        upperLimit,
        sum);
    assert(sum == 233168);
}
