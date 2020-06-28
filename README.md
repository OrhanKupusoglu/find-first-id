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

Ordered sets, [std::set](https://en.cppreference.com/w/cpp/container/set), provide two elegant solutions:
- keep track of used IDs, i.e. set is increasing
- keep track of available IDs, i.e. set is decreasing

As we will see during the benchmarks, this choice has a huge impact on the performance of this solution.

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

&nbsp;

## De Bruijn Sequence

On C++11/14/17 for a generic solution without using compiler built-in functions, [De Bruijn sequence](https://en.wikipedia.org/wiki/De_Bruijn_sequence) **B(2,6)** may be used with preprocessor directive **DE_BRUIJN_SEQUENCE**.

According to a few benchmark runs De Bruijn sequence performs only slightly less than the built-in function.

**DE_BRUIJN_SEQUENCE** declaration may be enabled on [CMakeVersion.txt](./src/CMakeVersion.txt).

The De Bruijn sequence table has been taken from [The Chess Programming Wiki](https://www.chessprogramming.org/BitScan#DeBruijnMultiplation).

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
$ cd src/

$ ./build.sh
-- The CXX compiler identification is GNU 7.5.0
-- Check for working CXX compiler: /usr/bin/c++
-- Check for working CXX compiler: /usr/bin/c++ -- works
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Detecting CXX compile features
-- Detecting CXX compile features - done
++ Compile definitions: KUPID_VERSION="0.2.1"
++ CMake build type: Release
++ C++ standard: 14
++ C++ flags: -O3 -DNDEBUG
++ will use __builtin_ffsll(x) to find the first zero bit
-- Configuring done
-- Generating done
-- Build files have been written to: /home/orhanku/ME/DEV/OK/temp/find-first-id/src/build

Scanning dependencies of target kupid
[ 50%] Building CXX object CMakeFiles/kupid.dir/src/main.cpp.o
[100%] Linking CXX executable ../kupid
[100%] Built target kupid

++ successfully built:
kupid - 28968 bytes

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

kupid::kset_inc
------------------------------------------------------------
++ size = 8192 : all used
++ last id is freed
get_id() = 8191
get_id() = -1
++ cleared
get_id() = 0

kupid::kset_dec
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
++ Compile definitions: KUPID_VERSION="0.2.1"
++ CMake build type: Release
++ C++ standard: 14
++ C++ flags: -O3 -DNDEBUG
++ will use __builtin_ffsll(x) to find the first zero bit
-- Found GTest: /usr/lib/libgtest.a
-- Configuring done
-- Generating done
-- Build files have been written to: /home/orhanku/ME/DEV/OK/temp/find-first-id/test/build

Scanning dependencies of target test-kupid
[ 14%] Building CXX object CMakeFiles/test-kupid.dir/src/main.cpp.o
[ 28%] Building CXX object CMakeFiles/test-kupid.dir/src/test_kbtree.cpp.o
[ 42%] Building CXX object CMakeFiles/test-kupid.dir/src/test_kbset.cpp.o
[ 57%] Building CXX object CMakeFiles/test-kupid.dir/src/test_kvector.cpp.o
[ 71%] Building CXX object CMakeFiles/test-kupid.dir/src/test_kset_inc.cpp.o
[ 85%] Building CXX object CMakeFiles/test-kupid.dir/src/test_kset_dec.cpp.o
[100%] Linking CXX executable ../test-kupid
[100%] Built target test-kupid

++ successfully built:
test-kupid - 1053840 bytes

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
TestKSetInc.
  SizeZero
  SizeOne
  SizeTwo
  ClearUseHalf
  SizeSmall
  SizeMedium
  SizeLarge
  RandomUnordered
  RandomOrdered
TestKSetDec.
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
[==========] 55 tests from 5 test cases ran. (2991 ms total)
[  PASSED  ] 55 tests.

## list tests
$ ./test-kupid --gtest_list_tests

## filter tests
$ ./test-kupid --gtest_filter=TestKBTree.BTree*
...
[----------] Global test environment tear-down
[==========] 10 tests from 1 test case ran. (3 ms total)
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
++ Compile definitions: KUPID_VERSION="0.2.1";BMARK_TEST_SIZE=8192
++ CMake build type: Release
++ C++ standard: 14
++ C++ flags: -O3 -DNDEBUG
++ will use __builtin_ffsll(x) to find the first zero bit
-- Configuring done
-- Generating done
-- Build files have been written to: /home/orhanku/ME/DEV/OK/temp/find-first-id/benchmark/build

Scanning dependencies of target bmark-kupid
[ 50%] Building CXX object CMakeFiles/bmark-kupid.dir/src/benchmark.cpp.o
[100%] Linking CXX executable ../bmark-kupid
[100%] Built target bmark-kupid

++ successfully built:
bmark-kupid - 1219000 bytes

++ test size: 8192
```

In the current Linux system two warnings are displayed:

```
Running ./bmark-kupid
Run on (4 X 3900 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x2)
  L1 Instruction 32 KiB (x2)
  L2 Unified 256 KiB (x2)
  L3 Unified 4096 KiB (x1)
Load Average: 2.16, 2.09, 1.99
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
***WARNING*** Library was built as DEBUG. Timings may be affected.
```

There are two scripts for test runs.

```
$ ./bmark.sh <test_dir> <test_format>

        test output files in 'test_format' will be saved into 'test_dir'

        default test_dir is 'runs', test_dir is created first
        default test_format is 'json', options are 'json|csv'
        runs benchmark with the given test sizes:
                1024
                4096
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

Benchmark results indicate that among the five classes the one using the std::set with decreasing size (start with all IDs as available, and remove used IDs), **kupid::kset_dec**, are faster than the others.

But there is an important caveat, **kupid::kset_dec** is slow to start.

**kupid::kbtree**, are faster than the three other classes. The difference grows as N goes to UINT32_MAX. The price is higher memory consumption due to position-indicating layers in addition to a more complex class.

The Big-O analysis indicates that, in the worst case (only last ID is available, N-1), **kupid::kbtree** algorithm is O(1):
Since the bit operations are O(1), a couple of calls make n x O(1), therefore the algorithm is of O(1) complexity.

Google Benchmark outputs prove that conclusion.

Results of the sample run below are presented in a table:

|Size<br>Class|1024<br>Time [ns]|4096<br>Time [ns]|8182<br>Time [ns]|16384<br>Time [ns]|32768<br>Time [ns]|65536<br>Time [ns]|131072<br>Time [ns]|1048576<br>Time [ns]|8388608<br>Time [ns]|16777216<br>Time [ns]|
|-----|--------:|---------:|--------:|---------:|--------:|---------:|--------:|---------:|---------:|---------:|
|kupid::kbtree|2.870|3.220|4.590|4.430|5.300|3.280|3.780|5.100|4.730|6.270|
|kupid::kvector|824.000|3,283.000|6,636.000|12,771.000|29,987.000|53,973.000|108,001.000|820,563.000|712,8804.000|14,173,959.000|
|kupid::kbset|841.000|3,241.000|6,633.000|13,487.000|28,080.000|53,390.000|102,465.000|812,895.000|661,8361.000|13,237,401.000|
|kupid::kset_inc|11,049.000|46,409.000|91,830.000|180,108.000|525,341.000|975,133.000|3,639,703.000|54,200,376.000|506,977,806.000|895,506,054.000|
|kupid::kset_dec|0.545|0.518|0.523|0.528|0.524|0.259|0.261|0.260|0.275|0.262|

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
++ test size: 1024
================================================================================
++ kupid::kbtree{1024}       : get_id() = 1023
++ kupid::kvector{1024}      : get_id() = 1023
++ kupid::kbset<1024>        : get_id() = 1023
++ kupid::kset_inc{1024}     : get_id() = 1023
++ kupid::kset_dec{1024}     : get_id() = 1023
------------------------------------------------------------
Benchmark                   Time             CPU   Iterations
-------------------------------------------------------------
benchmark_kbtree         2.87 ns         2.86 ns    265926072
benchmark_kvector         824 ns          824 ns       833033
benchmark_kbset           841 ns          841 ns       795868
benchmark_kset_inc      11049 ns        11044 ns        69642
benchmark_kset_dec      0.545 ns        0.545 ns   1000000000
================================================================================
++ test size: 4096
================================================================================
++ kupid::kbtree{4096}       : get_id() = 4095
++ kupid::kvector{4096}      : get_id() = 4095
++ kupid::kbset<4096>        : get_id() = 4095
++ kupid::kset_inc{4096}     : get_id() = 4095
++ kupid::kset_dec{4096}     : get_id() = 4095
-------------------------------------------------------------
Benchmark                   Time             CPU   Iterations
-------------------------------------------------------------
benchmark_kbtree         3.22 ns         3.22 ns    207803057
benchmark_kvector        3283 ns         3283 ns       190683
benchmark_kbset          3241 ns         3241 ns       212911
benchmark_kset_inc      46409 ns        46409 ns        15315
benchmark_kset_dec      0.518 ns        0.518 ns   1000000000
================================================================================
++ test size: 8192
================================================================================
++ kupid::kbtree{8192}       : get_id() = 8191
++ kupid::kvector{8192}      : get_id() = 8191
++ kupid::kbset<8192>        : get_id() = 8191
++ kupid::kset_inc{8192}     : get_id() = 8191
++ kupid::kset_dec{8192}     : get_id() = 8191
Benchmark                   Time             CPU   Iterations
-------------------------------------------------------------
benchmark_kbtree         4.59 ns         4.58 ns    167540845
benchmark_kvector        6636 ns         6636 ns       106343
benchmark_kbset          6633 ns         6626 ns       104496
benchmark_kset_inc      91830 ns        91824 ns         7557
benchmark_kset_dec      0.523 ns        0.522 ns   1000000000
================================================================================
++ test size: 16384
================================================================================
++ kupid::kbtree{16384}      : get_id() = 16383
++ kupid::kvector{16384}     : get_id() = 16383
++ kupid::kbset<16384>       : get_id() = 16383
++ kupid::kset_inc{16384}    : get_id() = 16383
++ kupid::kset_dec{16384}    : get_id() = 16383
------------------------------------------------------------
Benchmark                   Time             CPU   Iterations
-------------------------------------------------------------
benchmark_kbtree         4.43 ns         4.42 ns    167056217
benchmark_kvector       12771 ns        12749 ns        54957
benchmark_kbset         13487 ns        13474 ns        54931
benchmark_kset_inc     180108 ns       180097 ns         3539
benchmark_kset_dec      0.528 ns        0.528 ns   1000000000
================================================================================
++ test size: 32768
================================================================================
++ kupid::kbtree{32768}      : get_id() = 32767
++ kupid::kvector{32768}     : get_id() = 32767
++ kupid::kbset<32768>       : get_id() = 32767
++ kupid::kset_inc{32768}    : get_id() = 32767
++ kupid::kset_dec{32768}    : get_id() = 32767
------------------------------------------------------------
Benchmark                   Time             CPU   Iterations
-------------------------------------------------------------
benchmark_kbtree         5.30 ns         5.27 ns    131745462
benchmark_kvector       29987 ns        29969 ns        25445
benchmark_kbset         28080 ns        28064 ns        26498
benchmark_kset_inc     525341 ns       525323 ns         1510
benchmark_kset_dec      0.524 ns        0.524 ns   1000000000
================================================================================
++ test size: 65536
================================================================================
++ kupid::kbtree{65536}      : get_id() = 65535
++ kupid::kvector{65536}     : get_id() = 65535
++ kupid::kbset<65536>       : get_id() = 65535
++ kupid::kset_inc{65536}    : get_id() = 65535
++ kupid::kset_dec{65536}    : get_id() = 65535
-------------------------------------------------------------
Benchmark                   Time             CPU   Iterations
-------------------------------------------------------------
benchmark_kbtree         3.28 ns         3.28 ns    211851891
benchmark_kvector       53973 ns        53929 ns        13586
benchmark_kbset         53390 ns        53389 ns        13607
benchmark_kset_inc     975133 ns       975037 ns          946
benchmark_kset_dec      0.259 ns        0.259 ns   1000000000
================================================================================
++ test size: 131072
================================================================================
++ kupid::kbtree{131072}     : get_id() = 131071
++ kupid::kvector{131072}    : get_id() = 131071
++ kupid::kbset<131072>      : get_id() = 131071
++ kupid::kset_inc{131072}   : get_id() = 131071
++ kupid::kset_dec{131072}   : get_id() = 131071
------------------------------------------------------------
Benchmark                   Time             CPU   Iterations
-------------------------------------------------------------
benchmark_kbtree         3.78 ns         3.77 ns    189071346
benchmark_kvector      108001 ns       107997 ns         6096
benchmark_kbset        102465 ns       102385 ns         6761
benchmark_kset_inc    3639703 ns      3636457 ns          190
benchmark_kset_dec      0.261 ns        0.261 ns   1000000000
================================================================================
++ test size: 1048576
================================================================================
++ kupid::kbtree{1048576}    : get_id() = 1048575
++ kupid::kvector{1048576}   : get_id() = 1048575
++ kupid::kbset<1048576>     : get_id() = 1048575
++ kupid::kset_inc{1048576}  : get_id() = 1048575
++ kupid::kset_dec{1048576}  : get_id() = 1048575
-------------------------------------------------------------
Benchmark                   Time             CPU   Iterations
-------------------------------------------------------------
benchmark_kbtree         5.10 ns         5.10 ns    100000000
benchmark_kvector      820563 ns       820529 ns          839
benchmark_kbset        812895 ns       812189 ns          848
benchmark_kset_inc   54200376 ns     54191671 ns           10
benchmark_kset_dec      0.260 ns        0.260 ns   1000000000
================================================================================
++ test size: 8388608
================================================================================
++ kupid::kbtree{8388608}    : get_id() = 8388607
++ kupid::kvector{8388608}   : get_id() = 8388607
++ kupid::kbset<8388608>     : get_id() = 8388607
++ kupid::kset_inc{8388608}  : get_id() = 8388607
++ kupid::kset_dec{8388608}  : get_id() = 8388607
-------------------------------------------------------------
Benchmark                   Time             CPU   Iterations
-------------------------------------------------------------
benchmark_kbtree         4.73 ns         4.73 ns    146625039
benchmark_kvector     7128804 ns      7100775 ns          107
benchmark_kbset       6618361 ns      6607357 ns          107
benchmark_kset_inc  506977806 ns    505701830 ns            2
benchmark_kset_dec      0.275 ns        0.275 ns   1000000000
================================================================================
++ test size: 16777216
================================================================================
++ kupid::kbtree{16777216}   : get_id() = 16777215
++ kupid::kvector{16777216}  : get_id() = 16777215
++ kupid::kbset<16777216>    : get_id() = 16777215
++ kupid::kset_inc{16777216} : get_id() = 16777215
++ kupid::kset_dec{16777216} : get_id() = 16777215
-------------------------------------------------------------
Benchmark                   Time             CPU   Iterations
-------------------------------------------------------------
benchmark_kbtree         6.27 ns         6.27 ns    115108478
benchmark_kvector    14173959 ns     14171426 ns           54
benchmark_kbset      13237401 ns     13222227 ns           53
benchmark_kset_inc  895506054 ns    887713218 ns            1
benchmark_kset_dec      0.262 ns        0.262 ns   1000000000
================================================================================
```
