// KaprekarConstTest.cpp, (c) Herbert Meiler, April/May 2015, First Release

#include <time.h>

#include <cassert>
#include <iostream>

#include "KaprekarConst.h"

struct Collector
{
    explicit Collector(std::ostream& os) :
        os(os),
        count(0)
    {
    }

    int Count() const
    {
        return count;
    }

    template<typename DigitContainer>
	inline void operator()(const DigitContainer& kc) /* const */
	{
        ++count;

        //

		for (auto it = kc.rbegin(); it != kc.rend(); ++it)
		{
            char digit = (char)*it;
            digit += (digit < 10) ? '0' : ('A' - 10);

            os << digit;
		}

		os << std::endl;
	}

private:

    std::ostream& os;
    int count; // mutable?
};

void Generate(int length)
{
    assert(length > 0);

    std::ostream& os = std::cout;

    os << "Length:" << length << std::endl;

    const clock_t start = clock();

    Collector collector(os);

    KaprekarConst<10> kc; // decimal system
    kc.Generate(length, collector);

    const clock_t end = clock();

    const double time = ((double)(end - start)) / CLK_TCK;

    os << "Results:" << collector.Count() << std::endl;
    os << "Duration:" << time << " seconds" << std::endl;

    os << std::endl;
}

int
main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cout << "Usage: programname {length-of-kaprekarconst}" << std::endl;
    }

    for (int index = 1; index < argc; ++index)
    {
        const int length = atoi(argv[index]);

        if (length > 0)
        {
            Generate(length);
        }
    }

    return 0;
}
