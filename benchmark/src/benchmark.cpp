#include <iostream>
#include <benchmark/benchmark.h>

#include "../../src/include/kbtree.h"
#include "../../src/include/kvector.h"
#include "../../src/include/kbset.h"
#include "../../src/include/kset_inc.h"
#include "../../src/include/kset_dec.h"

// passed as a define, for example: -DBMARK_TEST_SIZE=1048576
constexpr uint32_t bmark_test_size = BMARK_TEST_SIZE;
constexpr uint32_t bmark_last_id = bmark_test_size - 1;

template <typename T>
class KFactory : public ::benchmark::Fixture {
    public:
        KFactory() : _id_factory{bmark_test_size} {};

        void SetUp(const ::benchmark::State& state) {
            for (uint32_t i = 0; i < bmark_test_size; ++i) {
                _id_factory.use_id(i);
            }

            _id_factory.free_id(bmark_last_id);
        }

        void TearDown(const ::benchmark::State& state) {
            _id_factory.clear();
        }

        T _id_factory;
};

using kbset_factory = kupid::kbset<bmark_test_size>;

template <>
class KFactory<kbset_factory> : public ::benchmark::Fixture {
    public:
        void SetUp(const ::benchmark::State& state) {
            for (uint32_t i = 0; i < bmark_test_size; ++i) {
                _id_factory.use_id(i);
            }

            _id_factory.free_id(bmark_last_id);
        }

        void TearDown(const ::benchmark::State& state) {
            _id_factory.clear();
        }

        kbset_factory _id_factory;
};

// print size and id

static void print_info() {
    std::cout << std::left << "++ size: " << BMARK_TEST_SIZE << " | last id: " <<  bmark_last_id << '\n';
    std::cout << "------------------------------------------------------------\n";
}

// register benchmarks

// -----------------------------------------------------------------------------
// kupid::kbtree

using benchmark_kbtree = KFactory<kupid::kbtree>;

BENCHMARK_DEFINE_F(benchmark_kbtree, test_kbtree)(benchmark::State& state) {
    uint32_t id;
    while (state.KeepRunning()) {
        benchmark::DoNotOptimize(id = _id_factory.get_id(false));
    }
}

#ifdef UNIT_MS
BENCHMARK_REGISTER_F(benchmark_kbtree, test_kbtree)->Unit(benchmark::kMillisecond);
#else
BENCHMARK_REGISTER_F(benchmark_kbtree, test_kbtree);
#endif

// -----------------------------------------------------------------------------
// kupid::kvector

using benchmark_kvector = KFactory<kupid::kvector>;

BENCHMARK_DEFINE_F(benchmark_kvector, test_kvector)(benchmark::State& state) {
    uint32_t id;
    while (state.KeepRunning()) {
        benchmark::DoNotOptimize(id = _id_factory.get_id(false));
    }
}

#ifdef UNIT_MS
BENCHMARK_REGISTER_F(benchmark_kvector, test_kvector)->Unit(benchmark::kMillisecond);
#else
BENCHMARK_REGISTER_F(benchmark_kvector, test_kvector);
#endif

// -----------------------------------------------------------------------------
// kupid::kbset<size>

using benchmark_kbset = KFactory<kupid::kbset<bmark_test_size>>;

BENCHMARK_DEFINE_F(benchmark_kbset, test_kbset)(benchmark::State& state) {
    uint32_t id;
    while (state.KeepRunning()) {
        benchmark::DoNotOptimize(id = _id_factory.get_id(false));
    }
}

#ifdef UNIT_MS
BENCHMARK_REGISTER_F(benchmark_kbset, test_kbset)->Unit(benchmark::kMillisecond);
#else
BENCHMARK_REGISTER_F(benchmark_kbset, test_kbset);
#endif

// -----------------------------------------------------------------------------
// kupid::kset_inc

using benchmark_kset_inc = KFactory<kupid::kset_inc>;

BENCHMARK_DEFINE_F(benchmark_kset_inc, kset_inc)(benchmark::State& state) {
    uint32_t id;
    while (state.KeepRunning()) {
        benchmark::DoNotOptimize(id = _id_factory.get_id(false));
    }
}

#ifdef UNIT_MS
BENCHMARK_REGISTER_F(benchmark_kset_inc, kset_inc)->Unit(benchmark::kMillisecond);
#else
BENCHMARK_REGISTER_F(benchmark_kset_inc, kset_inc);
#endif

// -----------------------------------------------------------------------------
// kupid::kset_dec

using benchmark_kset_dec = KFactory<kupid::kset_dec>;

BENCHMARK_DEFINE_F(benchmark_kset_dec, kset_dec)(benchmark::State& state) {
    uint32_t id;
    while (state.KeepRunning()) {
        benchmark::DoNotOptimize(id = _id_factory.get_id(false));
    }
}

#ifdef UNIT_MS
BENCHMARK_REGISTER_F(benchmark_kset_dec, kset_dec)->Unit(benchmark::kMillisecond);
#else
BENCHMARK_REGISTER_F(benchmark_kset_dec, kset_dec);
#endif

// run the benchmark
//BENCHMARK_MAIN();

// modify the macro above to print first size info
int main(int argc, char** argv) {
    print_info();

    ::benchmark::Initialize(&argc, argv);
    if (::benchmark::ReportUnrecognizedArguments(argc, argv)) return 1;
    ::benchmark::RunSpecifiedBenchmarks();
}
