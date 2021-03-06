#include <boost/text/string.hpp>

#include "string_objects.hpp"

#include <benchmark/benchmark.h>

#include <iostream>


void BM_string_erase_insert_front(benchmark::State & state)
{
    auto & string = strings[state.range(0)];
    auto const i = 0;
    while (state.KeepRunning()) {
        benchmark::DoNotOptimize(string.erase(string(i, i + 1)).insert(i, "."));
    }
}

void BM_string_erase_insert_back(benchmark::State & state)
{
    auto & string = strings[state.range(0)];
    auto const i = string.size() - 1;
    assert(0 <= i);
    while (state.KeepRunning()) {
        benchmark::DoNotOptimize(string.erase(string(i, i + 1)).insert(i, "."));
        assert(i == string.size() - 1);
    }
}

void BM_unencoded_rope_erase_insert_front(benchmark::State & state)
{
    auto & unencoded_rope = unencoded_ropes[state.range(0)];
    auto const i = 0;
    while (state.KeepRunning()) {
        benchmark::DoNotOptimize(
            unencoded_rope.erase(unencoded_rope(i, i + 1)).insert(i, "."));
    }
}

void BM_unencoded_rope_erase_insert_back(benchmark::State & state)
{
    auto & unencoded_rope = unencoded_ropes[state.range(0)];
    auto const i = unencoded_rope.size() - 1;
    assert(0 <= i);
    while (state.KeepRunning()) {
        benchmark::DoNotOptimize(
            unencoded_rope.erase(unencoded_rope(i, i + 1)).insert(i, "."));
        assert(i == unencoded_rope.size() - 1);
    }
}

BENCHMARK(BM_string_erase_insert_front) BENCHMARK_ARGS_NONEMPTY();
BENCHMARK(BM_string_erase_insert_back) BENCHMARK_ARGS_NONEMPTY();
BENCHMARK(BM_unencoded_rope_erase_insert_front) BENCHMARK_ARGS_NONEMPTY();
BENCHMARK(BM_unencoded_rope_erase_insert_back) BENCHMARK_ARGS_NONEMPTY();

BENCHMARK_MAIN()
