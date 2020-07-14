#include "gtest/gtest.h"
#include "../include/kcommon_tests.h"
#include "../../src/include/kvector.h"

kcommon_tests<kupid::kvector> test_kvector{"kupid::kvector"};

TEST(TestKVector, SizeZero) {
    test_kvector.test_size_zero();
}

TEST(TestKVector, SizeOne) {
    test_kvector.test_size_one();
}

TEST(TestKVector, SizeTwo) {
    test_kvector.test_size_two();
}

TEST(TestKVector, ClearUseHalf) {
    test_kvector.test_clear_use_half();
}

TEST(TestKVector, SizeSmall) {
    test_kvector.test_size_small();
}

TEST(TestKVector, SizeMedium) {
    test_kvector.test_size_medium();
}

TEST(TestKVector, SizeLarge) {
    test_kvector.test_size_large();
}

#ifdef TEST_XLARGE
TEST(TestKVector, SizeXLarge) {
    test_kvector.test_size_xlarge();
}
#endif

TEST(TestKVector, RandomUnordered) {
    test_kvector.test_random_unordered();
}

TEST(TestKVector, RandomOrdered) {
    test_kvector.test_random_ordered();
}
