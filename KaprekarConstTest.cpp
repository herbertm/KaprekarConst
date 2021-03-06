/*
 * Copyright (c) 2015-2016, Herbert Meiler
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 *   Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 *   Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

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
