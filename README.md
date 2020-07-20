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

$ cd ..
orhanku@orhan-lm19:~/ME/DEV/OK/find-first-id$ cd src/
orhanku@orhan-lm19:~/ME/DEV/OK/find-first-id/src$ ./build.sh
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
-- Build files have been written to: /home/orhanku/ME/DEV/OK/find-first-id/src/build

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
next() = 8191
next() = -1
++ cleared
next() = 0

kupid::kvector
------------------------------------------------------------
++ size = 8192 : all used
++ last id is freed
next() = 8191
next() = -1
++ cleared
next() = 0

kupid::kbset - templated
------------------------------------------------------------
++ size = 8192 : all used
++ last id is freed
next() = 8191
next() = -1
++ cleared
next() = 0

kupid::kset_inc
------------------------------------------------------------
++ size = 8192 : all used
++ last id is freed
next() = 8191
next() = -1
++ cleared
next() = 0

kupid::kset_dec
------------------------------------------------------------
++ size = 8192 : all used
++ last id is freed
next() = 8191
next() = -1
++ cleared
next() = 0

$ cd ..

## clean artifacts
$ ./clean.sh
```

This application can also be built with a simple command without cmake:

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
-- Build files have been written to: /home/orhanku/ME/DEV/OK/find-first-id/test/build

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
[==========] 55 tests from 5 test cases ran. (2398 ms total)
[  PASSED  ] 55 tests.

## list tests
$ ./test-kupid --gtest_list_tests

## filter tests
$ ./test-kupid --gtest_filter=TestKBTree.BTree*
...
[----------] Global test environment tear-down
[==========] 10 tests from 1 test case ran. (12 ms total)
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
-- Build files have been written to: /home/orhanku/ME/DEV/OK/find-first-id/benchmark/build

Scanning dependencies of target bmark-kupid
[ 50%] Building CXX object CMakeFiles/bmark-kupid.dir/src/benchmark.cpp.o
[100%] Linking CXX executable ../bmark-kupid
[100%] Built target bmark-kupid

++ successfully built:
bmark-kupid - 365384 bytes

++ test size: 8192
```

In the current Linux system one warning is displayed:

```
$ ./bmark-kupid
++ size: 8192 | last id: 8191
------------------------------------------------------------
Run on (4 X 3900 MHz CPU s)
2020-07-12 18:52:16
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
----------------------------------------------------------------------
Benchmark                               Time           CPU Iterations
----------------------------------------------------------------------
benchmark_kbtree/test_kbtree            4 ns          4 ns  203797718
benchmark_kvector/test_kvector       6415 ns       6415 ns     106702
benchmark_kbset/test_kbset           7844 ns       7844 ns      91044
benchmark_kset_inc/kset_inc         91028 ns      91017 ns       6722
benchmark_kset_dec/kset_dec             1 ns          1 ns  540519537
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

The success of **kupid::kset_dec** was unexpected.

The innovative **kupid::kbtree** is faster than the three other classes. The difference grows as N goes to UINT32_MAX. The price is higher memory consumption due to position-indicating layers in addition to a more complex class.

The Big-O analysis indicates that, in the worst case (only last ID is available, N-1), **kupid::kbtree** algorithm is O(1):
Since the bit operations are O(1), a couple of calls make n x O(1), therefore the algorithm is of O(1) complexity.

Google Benchmark outputs prove that conclusion.

Results of the sample run below are presented in a table:

|Size<br>Class|1,024=1KB<br>Time [ns]|4,096=4KB<br>Time [ns]|8,182=8KB<br>Time [ns]|16,384=16KB<br>Time [ns]|32,768=32KB<br>Time [ns]|65,536=64KB<br>Time [ns]|131,072=128KB<br>Time [ns]|1,048,576=1MB<br>Time [ns]|8,388,608=8MB<br>Time [ns]|16,777,216=16MB<br>Time [ns]|
|-----|--------:|---------:|--------:|---------:|--------:|---------:|--------:|---------:|---------:|---------:|
|kupid::kbtree|3|4|4|3|3|4|3|5|5|6|
|kupid::kvector|800|3,177|6,358|12,812|25,374|52,259|101,411|811,928|6,562,127|13,015,526|
|kupid::kbset|1,062|4,238|7,450|14,929|29,561|59,902|118,345|947,592|7,666,896|15,175,448|
|kupid::kset_inc|8,907|39,698|86,647|183,228|341,079|844,245|3,071,963|38,056,592|507,186,960|511,558,361|
|kupid::kset_dec|1|1|1|1|1|1|1|1|1|1|

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
CPU MHz:             800.017
CPU max MHz:         3900,0000
CPU min MHz:         400,0000
BogoMIPS:            5799.77
Virtualization:      VT-x
L1d cache:           32K
L1i cache:           32K
L2 cache:            256K
L3 cache:            4096K
NUMA node0 CPU(s):   0-3
Flags:               fpu vme de pse tsc msr pae mce cx8 apic sep mtrr pge mca cmov pat pse36 clflush dts acpi mmx fxsr sse sse2 ss ht tm pbe syscall nx pdpe1gb rdtscp lm constant_tsc art arch_perfmon pebs bts rep_good nopl xtopology nonstop_tsc cpuid aperfmperf pni pclmulqdq dtes64 monitor ds_cpl vmx smx est tm2 ssse3 sdbg fma cx16 xtpr pdcm pcid sse4_1 sse4_2 x2apic movbe popcnt tsc_deadline_timer aes xsave avx f16c rdrand lahf_lm abm 3dnowprefetch cpuid_fault epb invpcid_single pti ssbd ibrs ibpb stibp tpr_shadow vnmi flexpriority ept vpid ept_ad fsgsbase tsc_adjust bmi1 hle avx2 smep bmi2 erms invpcid rtm mpx rdseed adx smap clflushopt intel_pt xsaveopt xsavec xgetbv1 xsaves dtherm ida arat pln pts hwp hwp_notify hwp_act_window hwp_epp md_clear flush_l1d
```

&nbsp;

Full output of the benchmark runs given above at the table:

```
$ ./bmark.sh
================================================================================
BENCHMARK SIZES:
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
================================================================================
-- The CXX compiler identification is GNU 7.5.0
-- Check for working CXX compiler: /usr/bin/c++
-- Check for working CXX compiler: /usr/bin/c++ -- works
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Detecting CXX compile features
-- Detecting CXX compile features - done
++ Compile definitions: KUPID_VERSION="0.2.1";BMARK_TEST_SIZE=1024
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
bmark-kupid - 365384 bytes

++ test size: 1024
================================================================================
++ size: 1024 | last id: 1023
------------------------------------------------------------
Run on (4 X 3900 MHz CPU s)
2020-07-12 18:54:12
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
----------------------------------------------------------------------
Benchmark                               Time           CPU Iterations
----------------------------------------------------------------------
benchmark_kbtree/test_kbtree            3 ns          3 ns  260054640
benchmark_kvector/test_kvector        800 ns        800 ns     851081
benchmark_kbset/test_kbset           1062 ns       1062 ns     647908
benchmark_kset_inc/kset_inc          8907 ns       8908 ns      76105
benchmark_kset_dec/kset_dec             1 ns          1 ns  653927145
================================================================================
-- The CXX compiler identification is GNU 7.5.0
-- Check for working CXX compiler: /usr/bin/c++
-- Check for working CXX compiler: /usr/bin/c++ -- works
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Detecting CXX compile features
-- Detecting CXX compile features - done
++ Compile definitions: KUPID_VERSION="0.2.1";BMARK_TEST_SIZE=4096
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
bmark-kupid - 365384 bytes

++ test size: 4096
================================================================================
++ size: 4096 | last id: 4095
------------------------------------------------------------
Run on (4 X 3900 MHz CPU s)
2020-07-12 18:54:18
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
----------------------------------------------------------------------
Benchmark                               Time           CPU Iterations
----------------------------------------------------------------------
benchmark_kbtree/test_kbtree            4 ns          4 ns  202704442
benchmark_kvector/test_kvector       3177 ns       3177 ns     218700
benchmark_kbset/test_kbset           4238 ns       4238 ns     165122
benchmark_kset_inc/kset_inc         39698 ns      39670 ns      17886
benchmark_kset_dec/kset_dec             1 ns          1 ns  643737622
================================================================================
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
-- Build files have been written to: /home/orhanku/ME/DEV/OK/find-first-id/benchmark/build

Scanning dependencies of target bmark-kupid
[ 50%] Building CXX object CMakeFiles/bmark-kupid.dir/src/benchmark.cpp.o
[100%] Linking CXX executable ../bmark-kupid
[100%] Built target bmark-kupid

++ successfully built:
bmark-kupid - 365384 bytes

++ test size: 8192
================================================================================
++ size: 8192 | last id: 8191
------------------------------------------------------------
Run on (4 X 3900 MHz CPU s)
2020-07-12 18:54:24
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
----------------------------------------------------------------------
Benchmark                               Time           CPU Iterations
----------------------------------------------------------------------
benchmark_kbtree/test_kbtree            4 ns          4 ns  200934766
benchmark_kvector/test_kvector       6358 ns       6358 ns     106933
benchmark_kbset/test_kbset           7450 ns       7450 ns      92179
benchmark_kset_inc/kset_inc         86647 ns      86649 ns       7620
benchmark_kset_dec/kset_dec             1 ns          1 ns  544068558
================================================================================
-- The CXX compiler identification is GNU 7.5.0
-- Check for working CXX compiler: /usr/bin/c++
-- Check for working CXX compiler: /usr/bin/c++ -- works
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Detecting CXX compile features
-- Detecting CXX compile features - done
++ Compile definitions: KUPID_VERSION="0.2.1";BMARK_TEST_SIZE=16384
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
bmark-kupid - 365392 bytes

++ test size: 16384
================================================================================
++ size: 16384 | last id: 16383
------------------------------------------------------------
Run on (4 X 3900 MHz CPU s)
2020-07-12 18:54:30
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
----------------------------------------------------------------------
Benchmark                               Time           CPU Iterations
----------------------------------------------------------------------
benchmark_kbtree/test_kbtree            3 ns          3 ns  200201639
benchmark_kvector/test_kvector      12812 ns      12813 ns      54016
benchmark_kbset/test_kbset          14929 ns      14930 ns      45880
benchmark_kset_inc/kset_inc        183228 ns     183225 ns       3776
benchmark_kset_dec/kset_dec             1 ns          1 ns  534043239
================================================================================
-- The CXX compiler identification is GNU 7.5.0
-- Check for working CXX compiler: /usr/bin/c++
-- Check for working CXX compiler: /usr/bin/c++ -- works
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Detecting CXX compile features
-- Detecting CXX compile features - done
++ Compile definitions: KUPID_VERSION="0.2.1";BMARK_TEST_SIZE=32768
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
bmark-kupid - 365392 bytes

++ test size: 32768
================================================================================
++ size: 32768 | last id: 32767
------------------------------------------------------------
Run on (4 X 3900 MHz CPU s)
2020-07-12 18:54:36
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
----------------------------------------------------------------------
Benchmark                               Time           CPU Iterations
----------------------------------------------------------------------
benchmark_kbtree/test_kbtree            3 ns          3 ns  201752526
benchmark_kvector/test_kvector      25374 ns      25374 ns      27408
benchmark_kbset/test_kbset          29561 ns      29560 ns      23521
benchmark_kset_inc/kset_inc        341079 ns     341078 ns       1775
benchmark_kset_dec/kset_dec             1 ns          1 ns  541938877
================================================================================
-- The CXX compiler identification is GNU 7.5.0
-- Check for working CXX compiler: /usr/bin/c++
-- Check for working CXX compiler: /usr/bin/c++ -- works
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Detecting CXX compile features
-- Detecting CXX compile features - done
++ Compile definitions: KUPID_VERSION="0.2.1";BMARK_TEST_SIZE=65536
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
bmark-kupid - 365392 bytes

++ test size: 65536
================================================================================
++ size: 65536 | last id: 65535
------------------------------------------------------------
Run on (4 X 3900 MHz CPU s)
2020-07-12 18:54:42
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
----------------------------------------------------------------------
Benchmark                               Time           CPU Iterations
----------------------------------------------------------------------
benchmark_kbtree/test_kbtree            4 ns          4 ns  200389118
benchmark_kvector/test_kvector      52259 ns      52260 ns      13309
benchmark_kbset/test_kbset          59902 ns      59903 ns      11441
benchmark_kset_inc/kset_inc        844245 ns     844043 ns        860
benchmark_kset_dec/kset_dec             1 ns          1 ns  540350408
================================================================================
-- The CXX compiler identification is GNU 7.5.0
-- Check for working CXX compiler: /usr/bin/c++
-- Check for working CXX compiler: /usr/bin/c++ -- works
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Detecting CXX compile features
-- Detecting CXX compile features - done
++ Compile definitions: KUPID_VERSION="0.2.1";BMARK_TEST_SIZE=131072
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
bmark-kupid - 365392 bytes

++ test size: 131072
================================================================================
++ size: 131072 | last id: 131071
------------------------------------------------------------
Run on (4 X 3900 MHz CPU s)
2020-07-12 18:54:48
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
----------------------------------------------------------------------
Benchmark                               Time           CPU Iterations
----------------------------------------------------------------------
benchmark_kbtree/test_kbtree            3 ns          3 ns  201321897
benchmark_kvector/test_kvector     101411 ns     101413 ns       6770
benchmark_kbset/test_kbset         118345 ns     118345 ns       5843
benchmark_kset_inc/kset_inc       3071963 ns    3072020 ns        210
benchmark_kset_dec/kset_dec             1 ns          1 ns  544189869
================================================================================
-- The CXX compiler identification is GNU 7.5.0
-- Check for working CXX compiler: /usr/bin/c++
-- Check for working CXX compiler: /usr/bin/c++ -- works
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Detecting CXX compile features
-- Detecting CXX compile features - done
++ Compile definitions: KUPID_VERSION="0.2.1";BMARK_TEST_SIZE=1048576
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
bmark-kupid - 365400 bytes

++ test size: 1048576
================================================================================
++ size: 1048576 | last id: 1048575
------------------------------------------------------------
Run on (4 X 3900 MHz CPU s)
2020-07-12 18:54:55
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
----------------------------------------------------------------------
Benchmark                               Time           CPU Iterations
----------------------------------------------------------------------
benchmark_kbtree/test_kbtree            5 ns          5 ns  149280318
benchmark_kvector/test_kvector     811928 ns     811860 ns        849
benchmark_kbset/test_kbset         947592 ns     947521 ns        730
benchmark_kset_inc/kset_inc      38056592 ns   38051691 ns         17
benchmark_kset_dec/kset_dec             1 ns          1 ns  536645361
================================================================================
-- The CXX compiler identification is GNU 7.5.0
-- Check for working CXX compiler: /usr/bin/c++
-- Check for working CXX compiler: /usr/bin/c++ -- works
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Detecting CXX compile features
-- Detecting CXX compile features - done
++ Compile definitions: KUPID_VERSION="0.2.1";BMARK_TEST_SIZE=8388608
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
bmark-kupid - 365400 bytes

++ test size: 8388608
================================================================================
++ size: 8388608 | last id: 8388607
------------------------------------------------------------
Run on (4 X 3900 MHz CPU s)
2020-07-12 18:55:07
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
----------------------------------------------------------------------
Benchmark                               Time           CPU Iterations
----------------------------------------------------------------------
benchmark_kbtree/test_kbtree            5 ns          5 ns  150281066
benchmark_kvector/test_kvector    6562127 ns    6559931 ns        105
benchmark_kbset/test_kbset        7666896 ns    7666788 ns         91
benchmark_kset_inc/kset_inc     507186960 ns  507187567 ns          2
benchmark_kset_dec/kset_dec             1 ns          1 ns  544188871
================================================================================
-- The CXX compiler identification is GNU 7.5.0
-- Check for working CXX compiler: /usr/bin/c++
-- Check for working CXX compiler: /usr/bin/c++ -- works
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Detecting CXX compile features
-- Detecting CXX compile features - done
++ Compile definitions: KUPID_VERSION="0.2.1";BMARK_TEST_SIZE=16777216
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
bmark-kupid - 365400 bytes

++ test size: 16777216
================================================================================
++ size: 16777216 | last id: 16777215
------------------------------------------------------------
Run on (4 X 3900 MHz CPU s)
2020-07-12 18:56:01
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
----------------------------------------------------------------------
Benchmark                               Time           CPU Iterations
----------------------------------------------------------------------
benchmark_kbtree/test_kbtree            6 ns          6 ns  112631629
benchmark_kvector/test_kvector   13015526 ns   13013996 ns         54
benchmark_kbset/test_kbset       15175448 ns   15172165 ns         46
benchmark_kset_inc/kset_inc     511558361 ns  511495590 ns          1
benchmark_kset_dec/kset_dec             1 ns          1 ns  546464089
================================================================================
```
