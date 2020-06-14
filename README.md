# Find First ID

Within a given [0, N) range, find the first available ID as close to zero as possible.
* example: range [0, 4), 0 is used, therefore the next available ID is 1.

Free a given ID, this ID joins to the set of available IDs.
* example: range [0, 4), {0,1,3} are used, then 1 are 3 are freed, therefore the next available ID is 1.

&nbsp;

## CPU Support

The name [find-first-set, ffs](https://en.wikipedia.org/wiki/Find_first_set) is used for a [GNU GCC](https://gcc.gnu.org/onlinedocs/gcc/Other-Builtins.html) built-in function.

> In computer software and hardware, find first set (ffs) or find first one is a bit operation that, given an unsigned machine word, designates the index or position of the least significant bit set to one in the word counting from the least significant bit position.

> Most modern CPU instruction set architectures provide one or more of these as hardware operators; software emulation is usually provided for any that aren't available, either as compiler intrinsics or in system libraries.

These useful and popular extensions are standardized with C++20's new &lt;bit&gt; [include](https://en.cppreference.com/w/cpp/header/bit).

&nbsp;

## Data Structures

As N goes to numeric limits, the need for an efficient data structure to store the states of IDs becomes more apparent.

Bits are a logical way to store on and off data, therefore std::vector's [boolean specialization](https://en.cppreference.com/w/cpp/container/vector_bool) and [std::bitset](https://en.cppreference.com/w/cpp/utility/bitset) are likely to be useful data structures.

N is a template non-type parameter for std::bitset, therefore this is a constraint on the use of std::bitset.

Ordered sets, [std::set](https://en.cppreference.com/w/cpp/container/set), provide an elegant, but relatively slow solution.

&nbsp;

## Algorithm

A BitTree can be imagined as layers of bit streams, where each bit indicates positions of the free bits on the next layer.

As an example in range [0-64) an availabe ID will be efficiently found.

Although the BitTree class works with 64-bits, here a simplified 4-bit BitTree of 3-layers is drawn, where the first layer (index 0) holds the actual data.

Starting form the last layer, the third layer, indicates that the 3rd [nibble](https://en.wikipedia.org/wiki/Nibble) has a vacancy.

The second layer indicates that in next layer the 12th nibble has a vacancy.

In the first layer the position of the the vacant bit at 11th nibble, index 1, gives the next available ID.

`id = 11 x 4 + 1 = 45`

Starting from the last layer, if one can easily find the first available bit, reaching down the first data layer is easy.

On real-life 64-bit size bit streams one can use GNU GCC's **__builtin_ffsl()** or its C++20 counterparts to find empty positions.

This is far more efficient than looping over the data layer to find a vacant position, as measured by the benchmark application.

```

#2                                                                ||.|
                                                                    +
                                                                    +
                                                                    +
#1                                                     |||| |||| |||. ||||
                                                                    +
                                                                    +
                                                                    +
#0    |||| |||| |||| ||||    |||| |||| |||| ||||    |||| |||| |||| |.||    |||| |||| |||| ||||

```

With byte positions the [Endianness](https://en.wikipedia.org/wiki/Endianness) of the underlying hardware becomes important.

The current implementation assumes a **Little Endian** system.

On C++11/14/17 for a generic solution without using compiler built-in functions, [De Bruijn sequence](https://en.wikipedia.org/wiki/De_Bruijn_sequence) **B(2,6)** may be used with preprocessor directive **DE_BRUIJN_SEQUENCE**.

According to a few benchmark runs De Bruijn sequence performs only slightly less than the built-in function.

**DE_BRUIJN_SEQUENCE** declaration may be enabled on [CMakeVersion.txt](./src/CMakeVersion.txt).

&nbsp;

## Bit Operations

If bits are displayed the bit operations are better visualized.

Therefore a sample source is provided:

```
# cd src/src

$ g++ -std=c++14 bitops.cpp -o bitops

$ ./bitops help
usage:
        ./bitops <number>
        default number = 1085102592571150095
        applies and displays bit ops on the number

$ ./bitops
                            1085102592571150095 = 0x0F0F0F0F0F0F0F0F

 HIGHEST ADDRESS                                                              LOWEST ADDRESS
---------------+------------------------ LITTLE ENDIAN -----------------------+----------------
MOST SIGNIFICANT
---------------+--------------------------------------------------------------+----------------
               +      1       2       3       4       5       6       7       8
               0000111100001111000011110000111100001111000011110000111100001111 :  bits
               1111000011110000111100001111000011110000111100001111000011110000 : ~bits
               8       7       6       5       4       3       2       1      +
---------------+--------------------------------------------------------------+----------------
                                                                              LEAST SIGNIFICANT

__builtin_popcountll(x) : returns the number of 1-bits in x
        __builtin_popcountll(0x0F0F0F0F0F0F0F0F) = 32

__builtin_clzll(x) : returns the number of leading 0-bits in x, starting at the most significant bit position
warning - if x is 0, the result is undefined
        __builtin_clzll(0x0F0F0F0F0F0F0F0F) = 4

__builtin_ctzll(x) : returns the number of trailing 0-bits in x, starting at the least significant bit position
warning - if x is 0, the result is undefined
        __builtin_ctzll(0x0F0F0F0F0F0F0F0F) = 0

__builtin_ffsll(x) : returns one plus the index of the least significant 1-bit of x, or if x is zero, returns zero
        __builtin_ffsll(0x0F0F0F0F0F0F0F0F) = 1

__builtin_ffsll(~x) - 1 : returns position of the first zero bit in x
        __builtin_ffsll(0xF0F0F0F0F0F0F0F0) - 1 = 4
```

&nbsp;

## C++ Standard

The C++ Standard can be set with the [CMakeVersion.txt](./src/CMakeVersion.txt), default value is **C++14**.

For C++20 support [CMake 3.12](https://cmake.org/cmake/help/v3.12/prop_tgt/CXX_STANDARD.html) or later versions are required.

```
$ cd src

$ cat CMakeVersion.txt | grep BUILD_CPP_STANDARD
set(BUILD_CPP_STANDARD 14)
```

&nbsp;

## C++ Compiler

The C++ compiler can be set with the [cxx.sh](./src/cxx.sh), default compiler path is **/usr/bin/c++**.

```
$ cd src

$ cat cxx.sh | grep CMAKE_CXX_COMPILER
CMAKE_CXX_COMPILER=/usr/bin/c++
#CMAKE_CXX_COMPILER=/usr/bin/clang
```

&nbsp;

## Source

The four classes, [kupid::kbtree](./src/include/kbtree.h) and the other three control classes, are header-only C++ classes.

A shell script runs [CMake](https://cmake.org/), and then makes the sample application **kupid**.

```
$ ./build.sh
-- The CXX compiler identification is GNU 7.5.0
-- Check for working CXX compiler: /usr/bin/c++
-- Check for working CXX compiler: /usr/bin/c++ -- works
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Detecting CXX compile features
-- Detecting CXX compile features - done
++ Compile definitions: KUPID_VERSION="0.1.1"
++ CMake build type: Release
++ C++ standard: 14
++ C++ flags: -O3 -DNDEBUG
++ will use __builtin_ffsll(x) to find the first zero bit
-- Configuring done
-- Generating done
-- Build files have been written to: /home/orhanku/ME/DEV/OK/find-first-id/src/build

Scanning dependencies of target kupid
[ 50%] Building CXX object CMakeFiles/kupid.dir/src/main.cpp.o
[100%] Linking CXX executable ../kupid
[100%] Built target kupid

++ successfully built:
kupid - 28848 bytes

$ ./kupid help
usage:
        ./kupid <size>
        default size = 8192
        creates instances of id factories

$ ./kupid
kupid::kbtree
------------------------------------------------------------
++ size = 8192 : all used
++ last id is freed
get_id() = 8191
get_id() = -1
++ cleared
get_id() = 0

kupid::kvector
------------------------------------------------------------
++ size = 8192 : all used
++ last id is freed
get_id() = 8191
get_id() = -1
++ cleared
get_id() = 0

kupid::kbset - templated
------------------------------------------------------------
++ size = 8192 : all used
++ last id is freed
get_id() = 8191
get_id() = -1
++ cleared
get_id() = 0

kupid::kset
------------------------------------------------------------
++ size = 8192 : all used
++ last id is freed
get_id() = 8191
get_id() = -1
++ cleared
get_id() = 0

$ cd ..

## clean artifacts
$ ./clean.sh
```

This application can also be built with a simple command:

```
# cd src/src

$ g++ -std=c++14 main.cpp -o kupid
```

&nbsp;

## Google Test

Unit tests prepared with [Google Test](https://github.com/google/googletest/) can be built with its own build script.

```
$ cd test/

$ ./build.sh
-- The CXX compiler identification is GNU 7.5.0
-- Check for working CXX compiler: /usr/bin/c++
-- Check for working CXX compiler: /usr/bin/c++ -- works
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Detecting CXX compile features
-- Detecting CXX compile features - done
++ Compile definitions: KUPID_VERSION="0.1.1"
++ CMake build type: Release
++ C++ standard: 14
++ C++ flags: -O3 -DNDEBUG
++ will use __builtin_ffsll(x) to find the first zero bit
-- Found GTest: /usr/lib/libgtest.a
-- Configuring done
-- Generating done
-- Build files have been written to: /home/orhanku/ME/DEV/OK/find-first-id/test/build

Scanning dependencies of target test-kupid
[ 16%] Building CXX object CMakeFiles/test-kupid.dir/src/main.cpp.o
[ 33%] Building CXX object CMakeFiles/test-kupid.dir/src/test_kbtree.cpp.o
[ 50%] Building CXX object CMakeFiles/test-kupid.dir/src/test_kbset.cpp.o
[ 66%] Building CXX object CMakeFiles/test-kupid.dir/src/test_kset.cpp.o
[ 83%] Building CXX object CMakeFiles/test-kupid.dir/src/test_kvector.cpp.o
[100%] Linking CXX executable ../test-kupid
[100%] Built target test-kupid

++ successfully built:
test-kupid - 992552 bytes

++ list of tests:
TestKBTree.
  BTreeDivMod
  BTree64BitsIsFull
  BTree64BitsIsOn
  BTree64BitsSetBitOn
  BTree64BitsSetBitOff
  BTree64BitsFindFirstBit1
  BTree64BitsFindFirstBit2
  BTree64BitsFindFirstBit3
  BTreeSizeSlice
  BTreeDataByIndex
  SizeZero
  SizeOne
  SizeTwo
  ClearUseHalf
  SizeSmall
  SizeMedium
  SizeLarge
  RandomUnordered
  RandomOrdered
TestKBSet.
  SizeZero
  SizeOne
  SizeTwo
  ClearUseHalf
  SizeSmall
  SizeMedium
  SizeLarge
  RandomUnordered
  RandomOrdered
TestKSet.
  SizeZero
  SizeOne
  SizeTwo
  ClearUseHalf
  SizeSmall
  SizeMedium
  SizeLarge
  RandomUnordered
  RandomOrdered
TestKVector.
  SizeZero
  SizeOne
  SizeTwo
  ClearUseHalf
  SizeSmall
  SizeMedium
  SizeLarge
  RandomUnordered
  RandomOrdered

$ ./test-kupid
...
[----------] Global test environment tear-down
[==========] 46 tests from 4 test cases ran. (1653 ms total)
[  PASSED  ] 46 tests.

## list tests
$ ./test-kupid --gtest_list_tests

## filter tests
$ ./test-kupid --gtest_filter=TestKBTree.BTree*
...
[----------] Global test environment tear-down
[==========] 10 tests from 1 test case ran. (54 ms total)
[  PASSED  ] 10 tests.
```

&nbsp;

## Google Benchmark

[Google Benchmark](https://github.com/google/benchmark) compares the four classes, which are tested for correctness by Google Test.

```
$ cd benchmark/

$ ./build.sh
-- The CXX compiler identification is GNU 7.5.0
-- Check for working CXX compiler: /usr/bin/c++
-- Check for working CXX compiler: /usr/bin/c++ -- works
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Detecting CXX compile features
-- Detecting CXX compile features - done
++ Compile definitions: KUPID_VERSION="0.1.1";BMARK_TEST_SIZE=8192
++ CMake build type: Release
++ C++ standard: 14
++ C++ flags: -O3 -DNDEBUG
++ will use __builtin_ffsll(x) to find the first zero bit
-- Configuring done
-- Generating done
-- Build files have been written to: /home/orhanku/ME/DEV/OK/find-first-id/benchmark/build

Scanning dependencies of target bmark-kupid
[ 50%] Building CXX object CMakeFiles/bmark-kupid.dir/src/benchmark.cpp.o
[100%] Linking CXX executable ../bmark-kupid
[100%] Built target bmark-kupid

++ successfully built:
bmark-kupid - 1218880 bytes

++ test size: 8192

$ ./bmark-kupid
```

In the current Linux system two warnings are displayed:

```
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
***WARNING*** Library was built as DEBUG. Timings may be affected.
```

There are two scripts for test runs.

```
$ ./bmark.sh -h
usage:
        ./bmark.sh <test_dir> <test_format>

        test output files in 'test_format' will be saved into 'test_dir'

        default test_dir is '.', test_dir is created first
        default test_format is 'json', options are 'json|csv'
        runs benchmark with the given test sizes:
                8192
                16384
                32768
                65536
                131072
                1048576
                8388608
                16777216

$ ./loop.sh -h
        calls benchmarks in a loop of size 10
        each test run's results are collected into its own directory in 'runs' prefixed with 'run'
```

&nbsp;

## Results

Benchmark results indicate that among the four classes the one using the ffs bit operations, **kupid::kbtree**, are faster than the others.
The difference grows as N goes to UINT32_MAX.

The price is higher memory consumption due to position-indicating layers in addition to a more complex class.

The Big-O analysis indicates that, in the worst case (only last ID is available, N-1), these algorithm is O(1):

* Since the bit operations are O(1), a couple of calls make **n x O(1)**, therefore the algorithm is of **O(1)** complexity.

Google Benchmark outputs proves that conclusion.

Results of the sample run below are presented in a table:

|Size<br>Class|8182<br>Time [ns]|16384<br>Time [ns]|32768<br>Time [ns]|65536<br>Time [ns]|131072<br>Time [ns]|1048576<br>Time [ns]|8388608<br>Time [ns]|16777216<br>Time [ns]|
|-----|--------:|---------:|--------:|---------:|--------:|---------:|--------:|---------:|
|kupid::kbtree|4.66|4.74|6.00|3.27|3.93|5.40|5.15|6.01|
|kupid::kvector|6,955.00|13,995.00|28,090.00|56,297.00|103,145.00|879,168.00|6,712,895.00|13,383,140.00|
|kupid::kbset|6,354.00|12,800.00|25,591.00|51,435.00|102,422.00|817,514.00|6,537,404.00|12,982,684.00|
|kupid::kset|85,208.00|169,433.00|363,521.00|848,111.00|2,955,843.00|47,941,305.00|390,132,506.00|745,136,304.00|

All benchmarks are ran on a notebook with the given specification:

```
$ lscpu
Architecture:        x86_64
CPU op-mode(s):      32-bit, 64-bit
Byte Order:          Little Endian
CPU(s):              4
On-line CPU(s) list: 0-3
Thread(s) per core:  2
Core(s) per socket:  2
Socket(s):           1
NUMA node(s):        1
Vendor ID:           GenuineIntel
CPU family:          6
Model:               142
Model name:          Intel(R) Core(TM) i7-7600U CPU @ 2.80GHz
Stepping:            9
CPU MHz:             800.038
CPU max MHz:         3900,0000
CPU min MHz:         400,0000
BogoMIPS:            5799.77
Virtualization:      VT-x
L1d cache:           32K
L1i cache:           32K
L2 cache:            256K
L3 cache:            4096K
NUMA node0 CPU(s):   0-3
Flags:               fpu vme de pse tsc msr pae mce cx8 apic sep mtrr pge mca cmov pat pse36 clflush dts acpi mmx fxsr sse sse2 ss ht tm pbe syscall nx pdpe1gb rdtscp lm constant_tsc art arch_perfmon pebs bts rep_good nopl xtopology nonstop_tsc cpuid aperfmperf pni pclmulqdq dtes64 monitor ds_cpl vmx smx est tm2 ssse3 sdbg fma cx16 xtpr pdcm pcid sse4_1 sse4_2 x2apic movbe popcnt tsc_deadline_timer aes xsave avx f16c rdrand lahf_lm abm 3dnowprefetch cpuid_fault epb invpcid_single pti ssbd ibrs ibpb stibp tpr_shadow vnmi flexpriority ept vpid fsgsbase tsc_adjust bmi1 hle avx2 smep bmi2 erms invpcid rtm mpx rdseed adx smap clflushopt intel_pt xsaveopt xsavec xgetbv1 xsaves dtherm ida arat pln pts hwp hwp_notify hwp_act_window hwp_epp md_clear flush_l1d
```

&nbsp;

The sample benchmark's output:

```
$ ./bmark.sh
## simplified output ...
================================================================================
++ test size: 8192
================================================================================
++ kupid::kbtree{8192}       : get_id() = 8191
++ kupid::kvector{8192}      : get_id() = 8191
++ kupid::kbset<8192>        : get_id() = 8191
++ kupid::kset{8192}         : get_id() = 8191
------------------------------------------------------------
Benchmark                  Time             CPU   Iterations
------------------------------------------------------------
benchmark_kbtree        4.66 ns         4.66 ns    148776423
benchmark_kvector       6955 ns         6956 ns        80590
benchmark_kbset         6354 ns         6354 ns       104761
benchmark_kset         85208 ns        85211 ns         7605
================================================================================
++ test size: 16384
================================================================================
++ kupid::kbtree{16384}      : get_id() = 16383
++ kupid::kvector{16384}     : get_id() = 16383
++ kupid::kbset<16384>       : get_id() = 16383
++ kupid::kset{16384}        : get_id() = 16383
------------------------------------------------------------
Benchmark                  Time             CPU   Iterations
------------------------------------------------------------
benchmark_kbtree        4.74 ns         4.74 ns    114872730
benchmark_kvector      13995 ns        13977 ns        53615
benchmark_kbset        12800 ns        12800 ns        44779
benchmark_kset        169433 ns       169440 ns         3206
================================================================================
++ test size: 32768
================================================================================
++ kupid::kbtree{32768}      : get_id() = 32767
++ kupid::kvector{32768}     : get_id() = 32767
++ kupid::kbset<32768>       : get_id() = 32767
++ kupid::kset{32768}        : get_id() = 32767
------------------------------------------------------------
Benchmark                  Time             CPU   Iterations
------------------------------------------------------------
benchmark_kbtree        6.00 ns         5.96 ns    105600599
benchmark_kvector      28090 ns        28090 ns        26573
benchmark_kbset        25591 ns        25592 ns        26592
benchmark_kset        363521 ns       363528 ns         1772
================================================================================
++ test size: 65536
================================================================================
++ kupid::kbtree{65536}      : get_id() = 65535
++ kupid::kvector{65536}     : get_id() = 65535
++ kupid::kbset<65536>       : get_id() = 65535
++ kupid::kset{65536}        : get_id() = 65535
------------------------------------------------------------
Benchmark                  Time             CPU   Iterations
------------------------------------------------------------
benchmark_kbtree        3.27 ns         3.27 ns    182498035
benchmark_kvector      56297 ns        56299 ns        11250
benchmark_kbset        51435 ns        51416 ns        11225
benchmark_kset        848111 ns       848134 ns          761
================================================================================
++ test size: 131072
================================================================================
++ kupid::kbtree{131072}     : get_id() = 131071
++ kupid::kvector{131072}    : get_id() = 131071
++ kupid::kbset<131072>      : get_id() = 131071
++ kupid::kset{131072}       : get_id() = 131071
------------------------------------------------------------
Benchmark                  Time             CPU   Iterations
------------------------------------------------------------
benchmark_kbtree        3.93 ns         3.92 ns    151587737
benchmark_kvector     103145 ns       103104 ns         6224
benchmark_kbset       102422 ns       102269 ns         6611
benchmark_kset       2955843 ns      2955400 ns          230
================================================================================
++ test size: 1048576
================================================================================
++ kupid::kbtree{1048576}    : get_id() = 1048575
++ kupid::kvector{1048576}   : get_id() = 1048575
++ kupid::kbset<1048576>     : get_id() = 1048575
++ kupid::kset{1048576}      : get_id() = 1048575
------------------------------------------------------------
Benchmark                  Time             CPU   Iterations
------------------------------------------------------------
benchmark_kbtree        5.40 ns         5.40 ns    101064966
benchmark_kvector     879168 ns       879198 ns          671
benchmark_kbset       817514 ns       817533 ns          799
benchmark_kset      47941305 ns     47942421 ns           23
================================================================================
++ test size: 8388608
================================================================================
++ kupid::kbtree{8388608}    : get_id() = 8388607
++ kupid::kvector{8388608}   : get_id() = 8388607
++ kupid::kbset<8388608>     : get_id() = 8388607
++ kupid::kset{8388608}      : get_id() = 8388607
------------------------------------------------------------
Benchmark                  Time             CPU   Iterations
------------------------------------------------------------
benchmark_kbtree        5.15 ns         5.15 ns    144452450
benchmark_kvector    6712895 ns      6710912 ns           88
benchmark_kbset      6537404 ns      6537477 ns          106
benchmark_kset     390132506 ns    390139715 ns            2
================================================================================
++ test size: 16777216
================================================================================
++ kupid::kbtree{16777216}   : get_id() = 16777215
++ kupid::kvector{16777216}  : get_id() = 16777215
++ kupid::kbset<16777216>    : get_id() = 16777215
++ kupid::kset{16777216}     : get_id() = 16777215
Benchmark                  Time             CPU   Iterations
------------------------------------------------------------
benchmark_kbtree        6.01 ns         6.00 ns    100680098
benchmark_kvector   13383140 ns     13377734 ns           49
benchmark_kbset     12982684 ns     12983105 ns           54
benchmark_kset     745136304 ns    744643326 ns            1
================================================================================
```
