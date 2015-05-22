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

  9999 - 1111 = 8888

 -> 4 times 9  minus 4 times 1 -> 4 times 8
 
 The behaviour of the borrow-flag is also well defined.
 
 
 The histogram approach is optimized for histograms which can be imaging "shifted",
 since they generate the same difference.
 
 e.g.
 
521-125 = 632-236 = 743-347 etc.


 
 TODO: add more documentation....
 
 
Performance:
------------

Testenvironment:
Apple iMac, Intel Core i3 3,06 GHz
Windows 7

1. 50-digits -> 136 Results in 7 Minutes

63333333333333333333333317666666666666666666666664
97777777755555555111111108888888844444444222222221
99997777777555555511111108888888444444422222220001
99999997777775555551111108888884444442222220000001
99999999997777755555111108888844444222220000000001
99999999999997777555511108888444422220000000000001
99999999999999997775551108884442220000000000000001
99999999999999999997755108844220000000000000000001
99999999999999999999997508420000000000000000000001
88664433333333333333332199776666666666666666553312
88886666444433333332221999977776666666555533331112
88887766665444443222219999997777765555543333221112
97533333333333333333333308666666666666666666666421
97755333333333333333333108866666666666666666644221
97775553333333333333331108886666666666666664442221
97777555533333333333311108888666666666666444422221
97777755555333333333111108888866666666644444222221
97777775555553333331111108888886666664444442222221
97777777555555533311111108888888666444444422222221
99753333333333333333333308666666666666666666664201
99775533333333333333333108866666666666666666442201
99777555333333333333331108886666666666666644422201
99777755553333333333311108888666666666664444222201
99777775555533333333111108888866666666444442222201
99777777555555333331111108888886666644444422222201
99777777755555553311111108888888664444444222222201
99975333333333333333333308666666666666666666642001
99977553333333333333333108866666666666666664422001
99977755533333333333331108886666666666666444222001
98876654433333333333322099877666666666666554332111
99977775555333333333311108888666666666644442222001
98877665544333333333221099887766666666655443322111
99977777555553333333111108888866666664444422222001
98877766555443333332211099888776666665544433222111
99977777755555533331111108888886666444444222222001
98877776655554433322111099888877666554444332222111
99977777775555555311111108888888644444442222222001
98877777665555544221111099888887755444443322222111
99997533333333333333333308666666666666666666420001
99997755333333333333333108866666666666666644220001
99997775553333333333331108886666666666664442220001
99887665443333333333322099877666666666665543321101
99997777555533333333311108888666666666444422220001
99887766554433333333221099887766666666554433221101
99997777755555333333111108888866666644444222220001
99887776655544333332211099888776666655444332221101
99997777775555553331111108888886664444442222220001
99887777665555443322111099888877665544443322221101
99999753333333333333333308666666666666666664200001
99999775533333333333333108866666666666666442200001
99999777555333333333331108886666666666644422200001
99988766544333333333322099877666666666655433211001
99999777755553333333311108888666666664444222200001
99988776655443333333221099887766666665544332211001
99999777775555533333111108888866666444442222200001
99988777665554433332211099888776666554443322211001
98888766665444433322220999987777666555543333211111
99999777777555555331111108888886644444422222200001
99988777766555544322111099888877655444433222211001
98888776666554444222210999988777755554433332211111
99999975333333333333333308666666666666666642000001
99999977553333333333333108866666666666664422000001
99999977755533333333331108886666666666444222000001
99998876654433333333322099877666666666554332110001
99999977775555333333311108888666666644442222000001
99998877665544333333221099887766666655443322110001
99999977777555553333111108888866664444422222000001
99998877766555443332211099888776665544433222110001
99888876666544443322220999987777665555433332111101
99999977777755555531111108888886444444222222000001
99998877776655554422111099888877554444332222110001
99999997533333333333333308666666666666666420000001
99999997755333333333333108866666666666644220000001
99999997775553333333331108886666666664442220000001
99999887665443333333322099877666666665543321100001
99999997777555533333311108888666666444422220000001
99999887766554433333221099887766666554433221100001
99999997777755555333111108888866644444222220000001
99999887776655544332211099888776655444332221100001
99988887666654444322220999987777655554333321111001
99999999753333333333333308666666666666664200000001
99999999775533333333333108866666666666442200000001
99999999777555333333331108886666666644422200000001
99999988766544333333322099877666666655433211000001
99999999777755553333311108888666664444222200000001
99999988776655443333221099887766665544332211000001
99999999777775555533111108888866444442222200000001
99999988777665554432211099888776554443322211000001
99998888766665444422220999987777555543333211110001
99999999975333333333333308666666666666642000000001
99999999977553333333333108866666666664422000000001
99999999977755533333331108886666666444222000000001
99999998876654433333322099877666666554332110000001
99999999977775555333311108888666644442222000000001
99999998877665544333221099887766655443322110000001
99999999977777555553111108888864444422222000000001
99999998877766555442211099888775544433222110000001
99999999997533333333333308666666666666420000000001
99999999997755333333333108866666666644220000000001
99999999997775553333331108886666664442220000000001
99999999887665443333322099877666665543321100000001
99999999997777555533311108888666444422220000000001
99999999887766554433221099887766554433221100000001
99999999999753333333333308666666666664200000000001
99999999999775533333333108866666666442200000000001
99999999999777555333331108886666644422200000000001
99999999988766544333322099877666655433211000000001
99999999999777755553311108888664444222200000000001
99999999988776655443221099887765544332211000000001
99999999999975333333333308666666666642000000000001
99999999999977553333333108866666664422000000000001
99999999999977755533331108886666444222000000000001
99999999998876654433322099877666554332110000000001
99999999999977775555311108888644442222000000000001
99999999998877665544221099887755443322110000000001
99999999999997533333333308666666666420000000000001
99999999999997755333333108866666644220000000000001
99999999999997775553331108886664442220000000000001
99999999999887665443322099877665543321100000000001
99999999999999753333333308666666664200000000000001
99999999999999775533333108866666442200000000000001
99999999999999777555331108886644422200000000000001
99999999999988766544322099877655433211000000000001
99999999999999975333333308666666642000000000000001
99999999999999977553333108866664422000000000000001
99999999999999977755531108886444222000000000000001
99999999999998876654422099877554332110000000000001
99999999999999997533333308666666420000000000000001
99999999999999997755333108866644220000000000000001
99999999999999999753333308666664200000000000000001
99999999999999999775533108866442200000000000000001
99999999999999999975333308666642000000000000000001
99999999999999999977553108864422000000000000000001
99999999999999999997533308666420000000000000000001
99999999999999999999753308664200000000000000000001
99999999999999999999975308642000000000000000000001 


