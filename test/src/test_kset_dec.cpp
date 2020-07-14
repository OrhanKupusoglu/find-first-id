#include "gtest/gtest.h"
#include "../include/kcommon_tests.h"
#include "../../src/include/kset_dec.h"

kcommon_tests<kupid::kset_dec> test_kset_dec{"kupid::kset_dec"};

TEST(TestKSetDec, SizeZero) {
    test_kset_dec.test_size_zero();
}

TEST(TestKSetDec, SizeOne) {
    test_kset_dec.test_size_one();
}

TEST(TestKSetDec, SizeTwo) {
    test_kset_dec.test_size_two();
}

TEST(TestKSetDec, ClearUseHalf) {
    test_kset_dec.test_clear_use_half();
}

TEST(TestKSetDec, SizeSmall) {
    test_kset_dec.test_size_small();
}

TEST(TestKSetDec, SizeMedium) {
    test_kset_dec.test_size_medium();
}

TEST(TestKSetDec, SizeLarge) {
    test_kset_dec.test_size_large();
}

#ifdef TEST_XLARGE
TEST(TestKSetDec, SizeXLarge) {
    test_kset_dec.test_size_xlarge();
}
#endif

TEST(TestKSetDec, RandomUnordered) {
    test_kset_dec.test_random_unordered();
}

TEST(TestKSetDec, RandomOrdered) {
    test_kset_dec.test_random_ordered();
}
