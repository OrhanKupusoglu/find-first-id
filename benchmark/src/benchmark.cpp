#include <iostream>
#include <benchmark/benchmark.h>

#include "../../src/include/kbset.h"
#include "../../src/include/kbtree.h"
#include "../../src/include/kvector.h"
#include "../../src/include/kset_inc.h"
#include "../../src/include/kset_dec.h"

// passed as a define, for example: -DBMARK_TEST_SIZE=1048576
constexpr uint32_t bmark_test_size = BMARK_TEST_SIZE;
constexpr uint32_t bmark_last_id = bmark_test_size - 1;
constexpr int32_t bmark_print_width = 28;

// -----------------------------------------------------------------------------
// kupid::kbtree

static kupid::kbtree prep_kbtree() {
    kupid::kbtree id_factory{bmark_test_size};

    for (uint32_t i = 0; i < bmark_test_size; ++i) {
        id_factory.use_id(i);
    }

    id_factory.free_id(bmark_last_id);

    return id_factory;
}

static void print_kbtree(kupid::kbtree& id_factory) {
    auto id = id_factory.get_id(false);

    std::string name_size{"++ kupid::kbtree{"};
    name_size += std::to_string(bmark_test_size);
    name_size += "}";

    std::cout.width(bmark_print_width);
    std::cout << std::left << name_size << " : get_id() = " << id << '\n';
}

static void test_kbtree(kupid::kbtree& id_factory) {
    uint32_t id;
    benchmark::DoNotOptimize(id = id_factory.get_id(false));
}

static void benchmark_kbtree(benchmark::State& state) {
    auto id_factory = prep_kbtree();
    for (auto _ : state) {
        test_kbtree(id_factory);
    }
}

// -----------------------------------------------------------------------------
// kupid::kbset<size>

static kupid::kbset<bmark_test_size> prep_kbset() {
    kupid::kbset<bmark_test_size> id_factory{};

    for (uint32_t i = 0; i < bmark_test_size; ++i) {
        id_factory.use_id(i);
    }

    id_factory.free_id(bmark_last_id);

    return id_factory;
}

static void print_kbset(kupid::kbset<bmark_test_size>& id_factory) {
    auto id = id_factory.get_id(false);

    std::string name_size{"++ kupid::kbset<"};
    name_size += std::to_string(bmark_test_size);
    name_size += ">";

    std::cout.width(bmark_print_width);
    std::cout << std::left << name_size << " : get_id() = " << id << '\n';
}

static void test_kbset(kupid::kbset<bmark_test_size>& id_factory) {
    uint32_t id;
    benchmark::DoNotOptimize(id = id_factory.get_id(false));
}

static void benchmark_kbset(benchmark::State& state) {
    auto id_factory = prep_kbset();
    for (auto _ : state) {
        test_kbset(id_factory);
    }
}

// -----------------------------------------------------------------------------
// kupid::kvector

static kupid::kvector prep_kvector() {
    kupid::kvector id_factory{bmark_test_size};

    for (uint32_t i = 0; i < bmark_test_size; ++i) {
        id_factory.use_id(i);
    }

    id_factory.free_id(bmark_last_id);

    return id_factory;
}

static void print_kvector(kupid::kvector& id_factory) {
    auto id = id_factory.get_id(false);

    std::string name_size{"++ kupid::kvector{"};
    name_size += std::to_string(bmark_test_size);
    name_size += "}";

    std::cout.width(bmark_print_width);
    std::cout << std::left << name_size << " : get_id() = " << id << '\n';
}

static void test_kvector(kupid::kvector& id_factory) {
    uint32_t id;
    benchmark::DoNotOptimize(id = id_factory.get_id(false));
}

static void benchmark_kvector(benchmark::State& state) {
    auto id_factory = prep_kvector();
    for (auto _ : state) {
        test_kvector(id_factory);
    }
}

// -----------------------------------------------------------------------------
// kupid::kset_inc

static kupid::kset_inc prep_kset_inc() {
    kupid::kset_inc id_factory{bmark_test_size};

    for (uint32_t i = 0; i < bmark_test_size; ++i) {
        id_factory.use_id(i);
    }

    id_factory.free_id(bmark_last_id);

    return id_factory;
}

static void print_kset_inc(kupid::kset_inc& id_factory) {
    auto id = id_factory.get_id(false);

    std::string name_size{"++ kupid::kset_inc{"};
    name_size += std::to_string(bmark_test_size);
    name_size += "}";

    std::cout.width(bmark_print_width);
    std::cout << std::left << name_size << " : get_id() = " << id << '\n';
}

static void test_kset_inc(kupid::kset_inc& id_factory) {
    uint32_t id;
    benchmark::DoNotOptimize(id = id_factory.get_id(false));
}

static void benchmark_kset_inc(benchmark::State& state) {
    auto id_factory = prep_kset_inc();
    for (auto _ : state) {
        test_kset_inc(id_factory);
    }
}

// -----------------------------------------------------------------------------
// kupid::kset_dec

static kupid::kset_dec prep_kset_dec() {
    kupid::kset_dec id_factory{bmark_test_size};

    for (uint32_t i = 0; i < bmark_test_size; ++i) {
        id_factory.use_id(i);
    }

    id_factory.free_id(bmark_last_id);

    return id_factory;
}

static void print_kset_dec(kupid::kset_dec& id_factory) {
    auto id = id_factory.get_id(false);

    std::string name_size{"++ kupid::kset_dec{"};
    name_size += std::to_string(bmark_test_size);
    name_size += "}";

    std::cout.width(bmark_print_width);
    std::cout << std::left << name_size << " : get_id() = " << id << '\n';
}

static void test_kset_dec(kupid::kset_dec& id_factory) {
    uint32_t id;
    benchmark::DoNotOptimize(id = id_factory.get_id(false));
}

static void benchmark_kset_dec(benchmark::State& state) {
    auto id_factory = prep_kset_dec();
    for (auto _ : state) {
        test_kset_dec(id_factory);
    }
}

// -----------------------------------------------------------------------------
// print sample IDs

static void print_ids() {
    auto bmark_kbtree = prep_kbtree();
    print_kbtree(bmark_kbtree);

    auto bmark_kvector = prep_kvector();
    print_kvector(bmark_kvector);

    auto bmark_kbset = prep_kbset();
    print_kbset(bmark_kbset);

    auto bmark_kset_inc = prep_kset_inc();
    print_kset_inc(bmark_kset_inc);

    auto bmark_kset_dec = prep_kset_dec();
    print_kset_dec(bmark_kset_dec);

    std::cout << "------------------------------------------------------------\n";
}

// -----------------------------------------------------------------------------

// register the function as a benchmark
#ifdef UNIT_MS
BENCHMARK(benchmark_kbtree)->Unit(benchmark::kMillisecond);
BENCHMARK(benchmark_kvector)->Unit(benchmark::kMillisecond);
BENCHMARK(benchmark_kbset)->Unit(benchmark::kMillisecond);
BENCHMARK(benchmark_kset_inc)->Unit(benchmark::kMillisecond);
BENCHMARK(benchmark_kset_dec)->Unit(benchmark::kMillisecond);
#else
BENCHMARK(benchmark_kbtree);
BENCHMARK(benchmark_kvector);
BENCHMARK(benchmark_kbset);
BENCHMARK(benchmark_kset_inc);
BENCHMARK(benchmark_kset_dec);
#endif

// run the benchmark
//BENCHMARK_MAIN();

// modify the macro above to print first meta data
int main(int argc, char** argv) {
    print_ids();

    ::benchmark::Initialize(&argc, argv);
    if (::benchmark::ReportUnrecognizedArguments(argc, argv)) return 1;
    ::benchmark::RunSpecifiedBenchmarks();
}
