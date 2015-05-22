KaprekarConst
-------------

KaprekarConst.h: A template-class for generating Kaprekar-Const's via brute force (for BASE [2;16])
KaprekarConstTest.cpp: A command-line (test-) tool using KaprekarConst-template.

TODO: add parallel version

Compiler used: VC2015 RC, Simply create a C++ Console Project and add both files.

TODO: add VC2015 prj file.

Background:
-----------

See 
http://en.wikipedia.org/wiki/6174_%28number%29
for an introduction on the topic.

See also this beautiful work (of my uncle, in German):
http://www.rzbt.haw-hamburg.de/dankert/spezmath/html/Kaprekar-Konstanten.pdf


(brute-force) Algorithm:
------------------------

Normally programs use the following approach:

Loop over all numbers with a given length (these numbers are potential KaprekarConst-Candidates).
Reorder the digits of these numbers building the largest possible number, building the smallest possible number.
Calculate the difference between these two extreme-values and compare it with the Candidate.
On equality a KaprekarConst was found.

This approach has several disadvantages:
- The same differences are calculated more than once.
- ...


"My" algorithm:

As a starting point i don't use KaprekarConst-Candidates but already the sorted Extreme-Value(s)
(which can also be interpreted as the Histogram of digits of a number!).

For example:

When generating all 50-digit KaprekarConst's (in decimal system) these extreme values or Histograms
need only 10 (the base of the decimal system) integers of storage. For each digit i save it's frequency
inside the number. Such histograms can also be interpreted as fingerprints.

The frequency of each digit is saved instead of all digits.

If such a histogram is read from "left to right" with a 2-dimensional index,
the histogram can be interpreted as the largest possible number which can be created
by these digits.
If such a histogram is read from "right to left" with a 2-dimensional index,
the histogram can be interpreted as the smallest possible number which can be created
by these digits.

When building a difference on such extreme values in the first step only a fingerprint
(histogram) is generated. The calculation of the difference is immediately aborted, when
the fingerprint of the difference can never reach the fingerprint of the extreme value(s).

The calculation of differences is optimized since both values are order, e.g.

  9999
- 1111
------
  8888
 
 -> 4 times 9  minus 4 times 1 -> 4 times 8
 
 The histogram approach is optimized for histograms which can be imaging "shifted",
 since the generate the same difference.
 
 e.g.
 
521-125 = 632-236 = 743-347 etc.


 
 TODO: add more documentation....
 
 
 
 
 








