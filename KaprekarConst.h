// KaprekarConst.h, (c) Herbert Meiler, April/May 2015, First Release

#ifndef __KAPREKARCONST_H__
#define __KAPREKARCONST_H__

#include <cassert>
#include <algorithm>
#include <vector> // TODO: eliminate the need of vector

//

#if 0 // ATTENTION: IS THERE A BUILT-IN POPCOUNT?

#include <intrin.h>

inline int PopCount(int mask)
{
	return __popcnt(mask);
}

#else

// taken (indirectly) from the book "Hacker's Delight" via
// http://stackoverflow.com/questions/109023/how-to-count-the-number-of-set-bits-in-a-32-bit-integer

inline int PopCount(int mask)
{
	mask = mask - ((mask >> 1) & 0x55555555);
	mask = (mask & 0x33333333) + ((mask >> 2) & 0x33333333);
	return (((mask + (mask >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24;
}

#endif

//

template<int BASE>
inline bool IsKaprekarConstCandidate(int sumOfDigits)
{
	return true;
	// return (0 == (sumOfDigits % (BASE - 1))); // TODO: right?
}

template<>
inline bool IsKaprekarConstCandidate<10>(int sumOfDigits)
{
	return (0 == (sumOfDigits % 9));
}

// TODO: 
// deliver specializations of IsKaprekarConstCandidate for other base's, if 
// the general case "(0 == (sumOfDigits % (BASE - 1)))" can not be proofed.

//

// TODO: make the code beautiful (introduce some iterator classes and algorithms, etc.)

template<int BASE = 10> // TODO consider making BASE a runtime thing?
class KaprekarConst
{
public:

	static_assert(BASE >= 2, "");
	static_assert(BASE <= 16, "");

    // TODO: all the functions inside this class are currently stateless.
    // consider making them static oder extract them into algorithms.

	/*
	template<typename Collector>
	inline void Generate(int length, Collector& collector, int offset = 0, int step = 1) // Generate1
	{
		// this is an alternative implementation of Generate1a/Generate1b,
		// which is maybe better suited for parallelization?

		assert(length > 0);
        
		//

		const int minDifferentDigits = (1 == length) ? 1 : 2;
		const int maxDifferentDigits = (BASE > length) ? length : BASE;

		assert(minDifferentDigits <= maxDifferentDigits);

		for (int digitsDistributionMaskSubMask = 0 + offset;
			digitsDistributionMaskSubMask < (1 << (BASE - 1));
			digitsDistributionMaskSubMask += step)
		{
			const int differentDigits = PopCount(digitsDistributionMaskSubMask) + 1;

			if ((differentDigits >= minDifferentDigits) && (differentDigits <= maxDifferentDigits))
			{
				const int digitsDistributionMask = (digitsDistributionMaskSubMask << 1) | 1;

				Generate(length, differentDigits, digitsDistributionMask, collector);
			}
		}
	}*/

	template<typename Collector>
	inline void Generate(int length, Collector& collector) // Generate1a
	{
        assert(length > 0);

		const int minDifferentDigits = (1 == length) ? 1 : 2;
		const int maxDifferentDigits = (BASE > length) ? length : BASE;

		assert(minDifferentDigits <= maxDifferentDigits);

		for (int differentDigits = minDifferentDigits;
		    differentDigits <= maxDifferentDigits;
			++differentDigits)
		{
			// TODO: call a more specialized version of the next Generate,
			// if differentDigits == BASE for some optimizations.

			Generate(length, differentDigits, collector);
		}
	}

	template<typename Collector>
	inline void Generate(int length, int differentDigits, Collector& collector) // Generate1b
	{
		for (int digitsDistributionMask = (1 << differentDigits) - 1;
		    digitsDistributionMask < (1 << BASE);
			digitsDistributionMask += 2)
		{
			if (differentDigits == PopCount(digitsDistributionMask)) // TODO: try to eliminate this if and the need for PopCount
			{
				Generate(length, differentDigits, digitsDistributionMask,
					collector);
			}
		}
	}

protected:

	// TODO: 
    // optimize, combine functions Generate1(a/b), Generate2 and Generate3 into
	// one function using some indirection.

	template<typename Collector>
	inline void Generate( // Generate2
		int length,
		int differentDigits,
		int digitsDistributionMask,
		Collector& collector)
	{
		assert(length > 0);

		assert(differentDigits >= ((1 == length) ? 1 : 2));
		assert(differentDigits <= ((BASE > length) ? length : BASE));

		int distribution[BASE];
		std::fill(distribution + 1, distribution + differentDigits, 1);

		int& rest = distribution[0];
		rest = length - differentDigits + 1;

		//

		for (;;)
		{
			Generate(length, differentDigits, distribution, digitsDistributionMask,
				collector);

			//

			bool nextDistribution = false;

			for (int index = 1; index != differentDigits; ++index)
			{
				++distribution[index];
				--rest;

				if (rest == 0)
				{
					rest += distribution[index] - 1;
					distribution[index] = 1;
				}
				else
				{
					nextDistribution = true;
					break;
				}
			}

			if (!nextDistribution)
			{
				break;
			}
		}
	}

	template<typename Collector>
	inline void Generate( // Generate3
		int length,
		int differentDigits,
		const int* permutation, // TODO: parameter name is ugly..
		int digitsDistributionMask,
		Collector& collector)
	{
		// TODO: if differentDigits == BASE, then the following steps are not necessary...

		int distribution[BASE];
		std::fill(distribution, distribution + BASE, 0);

		int distributionValue[BASE];

		int sumOfDigits = 0;

		for (int digitValue = 0, distributionIndex = 0, mask = digitsDistributionMask;
		0 != mask; ++digitValue, mask >>= 1)
		{
			if (0 != (mask & 1))
			{
				const int frequency = permutation[distributionIndex];

				distribution[digitValue] = frequency;

				sumOfDigits += frequency * digitValue;

				//

				distributionValue[distributionIndex] = digitValue;

				//

				++distributionIndex;
			}
		}

		Generate(length, differentDigits, digitsDistributionMask, distribution,
			distributionValue, sumOfDigits, collector);
	}

	template<typename Collector>
	inline void Generate( // Generate4
		int length,
		int differentDigits,
		int digitsDistributionMask,
		const int* distribution,
		const int* distributionValue,
		int sumOfDigits,
		Collector& collector)
	{
		// TODO: use optimized version for the case differentDigits == BASE.

		int minDigitValueMask = 0;
		// int minDigitValueCount = 0;
		for (int minDigitValue = 0, mask = digitsDistributionMask;
		    (0 == (mask & 1 << BASE)); 
            ++minDigitValue, mask <<= 1)
		{
			if (IsKaprekarConstCandidate<BASE>(sumOfDigits + minDigitValue * length))
			{
				minDigitValueMask |= 1 << minDigitValue;
				// ++minDigitValueCount;
			}
		}

		//

		if (minDigitValueMask != 0)
		{
			// TODO: use simpler handler, for minDigitValueCount == 1.

			class SubtractHandler
			{
			public:

				inline SubtractHandler(const int* distribution, int& minDigitValueMask) :
					distribution(distribution),
					minDigitValueMask(minDigitValueMask)
				{
					::memset(resultDistribution, 0, BASE * sizeof(int));
				}

				inline bool operator()(int digitIndex, int digitValue, int digitCount)
				{
					int& resultFrequency = resultDistribution[digitValue];
					resultFrequency += digitCount;

					//

					for (int minDigitValue = 0, mask = minDigitValueMask;
					    mask != 0; 
                        ++minDigitValue, mask >>= 1)
					{
						if (0 != (mask & 1))
						{
							const int realDigitValue = digitValue - minDigitValue;
							assert(realDigitValue < BASE);

							const int frequency = (realDigitValue >= 0) ?
								distribution[realDigitValue] : -1;

							if (resultFrequency > frequency)
							{
								minDigitValueMask &= ~(1 << minDigitValue);
							}
						}
					}

					//

					return (0 != minDigitValueMask);
				}

			private:

				const int* distribution;
				int& minDigitValueMask;

				int resultDistribution[BASE];
			};

			SubtractHandler subtractHandler(distribution, minDigitValueMask);
			Subtract(length, differentDigits, distribution, distributionValue, subtractHandler);

			if (minDigitValueMask != 0)
			{
				// We found another Kaprekar constant. Calculate it!

				assert(1 == PopCount(minDigitValueMask));

				// TODO: get rid of dynamic memory allocation(vector) for the result.
                // TODO: consider using a different "interface" for Collector
				// TODO: consider to implement Subtract the "wrong way", from right to left.

				struct ResultHandler
				{
					inline explicit ResultHandler(int length) :
						result(length)
					{
					}

					inline bool operator()(int digitIndex, int digitValue, int digitCount)
					{
						auto it = result.begin() + digitIndex;
						std::fill(it, it + digitCount, digitValue);

						return true;
					}

					inline const std::vector<int>& Result() const
					{
						return result;
					}

				private:

					std::vector<int> result;
				};

				ResultHandler resultHandler(length);
				Subtract(length, differentDigits, distribution, distributionValue,
					resultHandler);

				collector(resultHandler.Result());
			}
		}
	}

	// TODO: Subtract doesn't depend on BASE, so extract it from the template.

    /*
    template<typename Handler>
    inline void Subtract( // SubtractSingleDigit (slower)
        int length,
        int differentDigits,
        const int* distribution,
        const int* distributionValue,
        Handler& handler)
    {
        // TODO: confusion, currently hi is lo and lo is hi. change this...

        const int* distributionValueHi = distributionValue + differentDigits - 1;
        int distributionValueHiIndex = 0;

        const int* distributionValueLo = distributionValue;
        int distributionValueLoIndex = 0;

        int borrow = 0;
        for (int digitIndex = 0; digitIndex != length; ++digitIndex)
        {
            int digitValue = *distributionValueLo - *distributionValueHi - borrow;

            if (digitValue < 0)
            {
                digitValue += BASE;
                borrow = 1;
            }
            else
            {
                borrow = 0;
            }

            //

            if (!handler(digitIndex, digitValue, 1))
            {
                break;
            }

            //

            if (++distributionValueHiIndex >= distribution[*distributionValueHi])
            {
                --distributionValueHi;
                distributionValueHiIndex = 0;
            }
            if (++distributionValueLoIndex >= distribution[*distributionValueLo])
            {
                ++distributionValueLo;
                distributionValueLoIndex = 0;
            }
        }
    }*/

	template<typename Handler>
	inline void Subtract( // SubtractRange (faster)
		int length,
		int differentDigits,
		const int* distribution,
		const int* distributionValue,
		Handler& handler)
	{
        assert(length > 0);

		int digitValue;

		// TODO: confusion, currently hi is lo and lo is hi. change this...
		const int* distributionValueHi = distributionValue + differentDigits - 1;
		const int* distributionValueLo = distributionValue;

		// first digit (borrow = 0)

        digitValue = *distributionValueLo - *distributionValueHi;

        if (digitValue == 0) // TODO: get rid of special treatment for length == 1
        {
            assert(length == 1);
            handler(0, digitValue, 1);

            return;
        }
        else
        {
            assert(digitValue < 0);
            assert(length > 1);

            digitValue += BASE;
        }

		assert(digitValue >= 0);
		assert(digitValue < BASE);

		if (!handler(0, digitValue, 1))
		{
			return;
		}

		// -> borrow = 1

		int digitIndex = 0 + 1;

		int distributionValueHiIndex = 0;
		int distributionValueLoIndex = 0;

		int distributionFrequencyHi = distribution[*distributionValueHi];
		int distributionFrequencyLo = distribution[*distributionValueLo];

		// TODO: consider using reference-parameter instead of reference-binding?
		auto nextHi = [&distributionValueHi, &distributionValueHiIndex, &distributionFrequencyHi, distribution](int count = 1)
		{
			if ((distributionValueHiIndex += count) >= distributionFrequencyHi)
			{
				--distributionValueHi;
				distributionValueHiIndex = 0;

				distributionFrequencyHi = distribution[*distributionValueHi]; // dangerous
			}
		};

		// TODO: consider using reference-parameter instead of reference-binding?
		auto nextLo = [&distributionValueLo, &distributionValueLoIndex, &distributionFrequencyLo, distribution](int count = 1)
		{
			if ((distributionValueLoIndex += count) >= distributionFrequencyLo)
			{
				++distributionValueLo;
				distributionValueLoIndex = 0;

				distributionFrequencyLo = distribution[*distributionValueLo]; // dangerous
			}
		};

		nextHi();
		nextLo();

		//

		while (*distributionValueLo <= *distributionValueHi)
		{
			const int digitCount = std::min(
				distributionFrequencyHi - distributionValueHiIndex,
				distributionFrequencyLo - distributionValueLoIndex);
			assert(digitCount > 0);

			digitValue = *distributionValueLo - *distributionValueHi - 1 + BASE;
			assert(digitValue >= 0);
			assert(digitValue < BASE);

			if (!handler(digitIndex, digitValue, digitCount))
			{
				return;
			}

			nextHi(digitCount);
			nextLo(digitCount);

			digitIndex += digitCount;
		}

		assert(digitIndex < (length - 1));

		// -> borrow == 1

		digitValue = *distributionValueLo - *distributionValueHi - 1;
		assert(digitValue >= 0);
		assert(digitValue < BASE);

		if (!handler(digitIndex, digitValue, 1))
		{
			return;
		}

		if (++digitIndex == length)
		{
			return;
		}

		//

		nextHi();
		nextLo();

		//

		// -> borrow == 0;

		for (;;)
		{
			const int digitCount = std::min(
				distributionFrequencyHi - distributionValueHiIndex,
				distributionFrequencyLo - distributionValueLoIndex);
			assert(digitCount > 0);

			digitValue = *distributionValueLo - *distributionValueHi;
			assert(digitValue >= 0);
			assert(digitValue < BASE);

			//

			if (!handler(digitIndex, digitValue, digitCount))
			{
				return;
			}

			//

			digitIndex += digitCount;
			if (digitIndex == length)
			{
				return;
			}

			assert(digitIndex < length);

			//

			nextHi(digitCount);
			nextLo(digitCount);
		}
	}
};

#endif
