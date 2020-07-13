#include "gtest/gtest.h"
#include "../include/kgeneric.h"
#include "../../src/include/kset_inc.h"

kgeneric<kupid::kset_inc> test_kset_inc{"kupid::kset_inc"};

TEST(TestKSetInc, SizeZero) {
    test_kset_inc.test_size_zero();
}

TEST(TestKSetInc, SizeOne) {
    test_kset_inc.test_size_one();
}

TEST(TestKSetInc, SizeTwo) {
    test_kset_inc.test_size_two();
}

TEST(TestKSetInc, ClearUseHalf) {
    test_kset_inc.test_clear_use_half();
}

TEST(TestKSetInc, SizeSmall) {
    test_kset_inc.test_size_small();
}

TEST(TestKSetInc, SizeMedium) {
    test_kset_inc.test_size_medium();
}

TEST(TestKSetInc, SizeLarge) {
    test_kset_inc.test_size_large();
}

#ifdef TEST_XLARGE
TEST(TestKSetInc, SizeXLarge) {
    test_kset_inc.test_size_xlarge();
}
#endif

TEST(TestKSetInc, RandomUnordered) {
    test_kset_inc.test_random_unordered();
}

TEST(TestKSetInc, RandomOrdered) {
    test_kset_inc.test_random_ordered();
}
