#include "gtest/gtest.h"
#include <bitset>
#include "../include/krandom.h"
#include "../../src/include/kbtree.h"

TEST(TestKBTree, BTreeDivMod) {
    using num_div_mod_t = std::tuple<uint32_t, uint32_t, uint32_t>;
    std::vector<num_div_mod_t> test_inputs = {{    0,    0,  0},
                                              {    1,    0,  1},
                                              {   63,    0, 63},
                                              {   64,    1,  0},
                                              {  127,    1, 63},
                                              {  128,    2,  0},
                                              {65535, 1023, 63},
                                              {65536, 1024,  0}};

    kupid::kbtree id_factory{0};

    for (const auto& num : test_inputs) {
        auto div_mod = id_factory.get_div_and_mod_by_64(std::get<0>(num));

        std::cout.width(8);
        std::cout << std::right << std::get<0>(num) << " = ";
        std::cout.width(6);
        std::cout << std::right << std::get<1>(num) << " x 64 + " << std::get<2>(num) << '\n';

        ASSERT_EQ(div_mod.div, std::get<1>(num));
        ASSERT_EQ(div_mod.mod, std::get<2>(num));
    }
}

TEST(TestKBTree, BTree64BitsIsFull) {
    std::vector<uint64_t> test_inputs = {0x0,
                                         0x1,
                                         0xFF,
                                         0xFFF,
                                         0xFFFF,
                                         0xFFFFFFF,
                                         0xFFFFFFFF,
                                         0xFFFFFFFFFFFF,
                                         0xFFFFFFFFFFFFFFFF};

    kupid::kbtree id_factory{0};

    for (const auto& num : test_inputs) {
        std::bitset<64> bits(num);
        std::cout << bits;

        if (num == 0xFFFFFFFFFFFFFFFF) {
            std::cout << " - full" << '\n';
            ASSERT_TRUE(id_factory.is_full(num));
        } else {
            std::cout << " - not full" << '\n';
            ASSERT_FALSE(id_factory.is_full(num));
        }
    }
}

TEST(TestKBTree, BTree64BitsIsOn) {
    using num_bit_on_t = std::tuple<uint64_t, uint64_t, bool>;
    std::vector<num_bit_on_t> test_inputs = {{               0x0,  0, false},
                                             {               0x1,  0, true},
                                             {              0x10,  4, true},
                                             {             0x100,  7, false},
                                             {             0x100,  8, true},
                                             {        0x10000000, 28, true},
                                             {    0x100000000000, 44, true},
                                             {0x1000000000000000, 60, true},
                                             {0x4000000000000000, 62, true},
                                             {0x4000000000000000, 63, false},
                                             {0x8000000000000000, 63, true}};

    kupid::kbtree id_factory{0};

    for (const auto& num : test_inputs) {
        auto num64 = std::get<0>(num);
        auto index = std::get<1>(num);
        auto is_on = std::get<2>(num);

        std::bitset<64> bits(num64);
        std::cout << bits << " - bit[" << index << "] : " << std::boolalpha <<  is_on << '\n';

        if (is_on) {
            ASSERT_TRUE(id_factory.is_bit_on(num64, index));
        } else {
            ASSERT_FALSE(id_factory.is_bit_on(num64, index));
        }
    }
}

TEST(TestKBTree, BTree64BitsSetBitOn) {
    kupid::kbtree id_factory{0};

    for (int i = 0; i < 64; ++i) {
        uint64_t num = 0;

        id_factory.set_bit_on(num, i);

        std::bitset<64> bits(num);
        std::cout << bits << " - bit[" << i << "] : " << std::boolalpha << true << '\n';

        ASSERT_TRUE(id_factory.is_bit_on(num, i));
    }
}

TEST(TestKBTree, BTree64BitsSetBitOff) {
    kupid::kbtree id_factory{0};

    for (int i = 0; i < 64; ++i) {
        uint64_t num = 0xFFFFFFFFFFFFFFFF;

        id_factory.set_bit_off(num, i);

        std::bitset<64> bits(num);
        std::cout << bits << " - bit[" << i << "] : " << std::boolalpha << false << '\n';

        ASSERT_FALSE(id_factory.is_bit_on(num, i));
    }
}

TEST(TestKBTree, BTree64BitsFindFirstBit1) {
    kupid::kbtree id_factory{0};

    for (int i = 0; i < 64; ++i) {
        uint64_t num = 0xFFFFFFFFFFFFFFFF;

        id_factory.set_bit_off(num, i);

        std::bitset<64> bits(num);
        std::cout << bits << " - bit[" << i << "] : first free bit\n";

        ASSERT_EQ(id_factory.find_first_free_bit(num), i);
    }
}

TEST(TestKBTree, BTree64BitsFindFirstBit2) {
    kupid::kbtree id_factory{0};

    for (int i = 0; i < 64; ++i) {
        uint64_t num = 0;

        for (int j = 0; j < i; ++j) {
            id_factory.set_bit_on(num, j);
        }

        std::bitset<64> bits(num);
        std::cout << bits << " - bit[" << i << "] : first free bit\n";

        ASSERT_EQ(id_factory.find_first_free_bit(num), i);
    }
}

TEST(TestKBTree, BTree64BitsFindFirstBit3) {
    kupid::kbtree id_factory{0};
    kupid::krandom_int rnd_factory{64};

    for (uint32_t i = 0; i < 64; ++i) {
        uint64_t num = 0;
        uint32_t index = 0;

        for (uint32_t j = 0; j < i; ++j) {
            id_factory.set_bit_on(num, j);
        }

        auto r = rnd_factory.get_random();
        id_factory.set_bit_off(num, r);

        auto k = id_factory.find_first_free_bit(num);

        if (r > i) {
            index = i;
        } else {
            index = r;
        }

        std::bitset<64> bits(num);
        std::cout << bits << " - bit[" << index << "] : first free bit\n";

        ASSERT_EQ(k, index);
    }
}

TEST(TestKBTree, BTreeSizeSlice) {
    uint32_t size = 352;

    std::cout << "test kupid::kbtree with size = " << size << '\n';

    kupid::kbtree id_factory{size};

    ASSERT_EQ(id_factory.size(), size);

    auto dm = id_factory.get_div_and_mod_by_64(size);
    auto slice = dm.mod > 0 ? dm.div + 1 : dm.div;

    std::cout << "slice = " << slice << '\n';

    ASSERT_EQ(id_factory.slice(), slice);
}

TEST(TestKBTree, BTreeDataByIndex) {
    uint32_t size = 352;

    std::cout << "test kupid::kbtree with size = " << size << '\n';

    kupid::kbtree id_factory{size};

    ASSERT_EQ(id_factory.size(), size);

    for (uint32_t i = 0; i < size; ++i) {
        id_factory.use_id(i);
    }

    auto last_1 = id_factory.get_data(id_factory.size() - 1, false);
    std::bitset<64> bits(last_1);
    std::cout << bits << " - last data chunk : get_data(" << (id_factory.size() - 1) << ", false)\n";

    auto last_2 = id_factory.get_data(id_factory.slice() - 1, true);
    bits = std::bitset<64>(last_2);
    std::cout << bits << " - last data chunk : get_data(" << (id_factory.slice() - 1) << ", true)\n";

    ASSERT_EQ(last_1, last_2);
}

// common tests

TEST(TestKBTree, SizeZero) {
    uint32_t size = 0;

    std::cout << "test kupid::kbtree with size = " << size << '\n';

    kupid::kbtree id_factory{size};

    auto id = id_factory.get_id();
    std::cout << "#1. id = " << id << '\n';
    ASSERT_EQ(id, -1);
}

TEST(TestKBTree, SizeOne) {
    uint32_t size = 1;

    std::cout << "test kupid::kbtree with size = " << size << '\n';

    kupid::kbtree id_factory{size};

    auto id = id_factory.get_id(false);
    std::cout << "#1. id = " << id << " - not marked as used\n";
    ASSERT_EQ(id, 0);

    id = id_factory.get_id();
    std::cout << "#1. id = " << id << '\n';
    ASSERT_EQ(id, 0);

    id = id_factory.get_id();
    std::cout << "#2. id = " << id << '\n';
    ASSERT_EQ(id, -1);
}

TEST(TestKBTree, SizeTwo) {
    uint32_t size = 2;

    std::cout << "test kupid::kbtree with size = " << size << '\n';

    kupid::kbtree id_factory{size};

    auto id = id_factory.get_id();
    std::cout << "#1. id = " << id << '\n';
    ASSERT_EQ(id, 0);

    id = id_factory.get_id();
    std::cout << "#2. id = " << id << '\n';
    ASSERT_EQ(id, 1);

    id = id_factory.get_id();
    std::cout << "#3. id = " << id << '\n';
    ASSERT_EQ(id, -1);
}

TEST(TestKBTree, ClearUseHalf) {
    uint32_t size = 1 * 1024;

    std::cout << "test kupid::kbtree with size = " << size << '\n';

    kupid::kbtree id_factory{size};

    ASSERT_EQ(id_factory.size(), size);

    for (uint32_t i = 0; i < size; ++i) {
        id_factory.use_id(i);
    }

    std::cout << "id_factory is full\n";

    auto id = id_factory.get_id();
    std::cout << "#1. id = " << id << '\n';
    ASSERT_EQ(id, -1);

    id_factory.clear();

    std::cout << "id_factory is cleared\n";

    id = id_factory.get_id();
    std::cout << "#2. id = " << id << '\n';
    ASSERT_EQ(id, 0);

    auto mid = size / 2;

    for (uint32_t i = 0; i < mid; ++i) {
        id_factory.use_id(i);
    }

    std::cout << "id_factory is 1/2 used\n";

    id = id_factory.get_id();
    std::cout << "#3. id = " << id << '\n';
    ASSERT_EQ(id, mid);
}

TEST(TestKBTree, SizeSmall) {
    uint32_t size = 1 * 1024;

    std::cout << "test kupid::kbtree with size = " << size << '\n';

    kupid::kbtree id_factory{size};

    ASSERT_EQ(id_factory.size(), size);

    for (uint32_t i = 0; i < size; ++i) {
        id_factory.use_id(i);
    }

    auto mid = size / 2;

    id_factory.free_id(0);
    id_factory.free_id(mid);
    id_factory.free_id(size - 1);

    ASSERT_FALSE(id_factory.is_using(0));
    ASSERT_FALSE(id_factory.is_using(mid));
    ASSERT_FALSE(id_factory.is_using(size - 1));

    auto id = id_factory.get_id();
    std::cout << "#1. id = " << id << '\n';
    ASSERT_EQ(id, 0);

    id = id_factory.get_id();
    std::cout << "#2. id = " << id << '\n';
    ASSERT_EQ(id, mid);

    id = id_factory.get_id();
    std::cout << "#3. id = " << id << '\n';
    ASSERT_EQ(id, size - 1);

    ASSERT_TRUE(id_factory.is_using(0));
    ASSERT_TRUE(id_factory.is_using(mid));
    ASSERT_TRUE(id_factory.is_using(size - 1));

    id = id_factory.get_id();
    std::cout << "#4. id = " << id << '\n';
    ASSERT_EQ(id, -1);

    ASSERT_FALSE(id_factory.free_id(size));
    ASSERT_FALSE(id_factory.use_id(size));
    ASSERT_FALSE(id_factory.is_using(size));

    ASSERT_TRUE(id_factory.free_id(size - 1));
    ASSERT_TRUE(id_factory.use_id(size - 1));
    ASSERT_TRUE(id_factory.is_using(size - 1));
}

TEST(TestKBTree, SizeMedium) {
    uint32_t size = 64 * 1024;

    std::cout << "test kupid::kbtree with size = " << size << '\n';

    kupid::kbtree id_factory{size};

    ASSERT_EQ(id_factory.size(), size);

    for (uint32_t i = 0; i < size; ++i) {
        id_factory.use_id(i);
    }

    auto mid = size / 2;

    id_factory.free_id(0);
    id_factory.free_id(mid);
    id_factory.free_id(size - 1);

    ASSERT_FALSE(id_factory.is_using(0));
    ASSERT_FALSE(id_factory.is_using(mid));
    ASSERT_FALSE(id_factory.is_using(size - 1));

    auto id = id_factory.get_id();
    std::cout << "#1. id = " << id << '\n';
    ASSERT_EQ(id, 0);

    id = id_factory.get_id();
    std::cout << "#2. id = " << id << '\n';
    ASSERT_EQ(id, mid);

    id = id_factory.get_id();
    std::cout << "#3. id = " << id << '\n';
    ASSERT_EQ(id, size - 1);

    ASSERT_TRUE(id_factory.is_using(0));
    ASSERT_TRUE(id_factory.is_using(mid));
    ASSERT_TRUE(id_factory.is_using(size - 1));

    id = id_factory.get_id();
    std::cout << "#4. id = " << id << '\n';
    ASSERT_EQ(id, -1);

    ASSERT_FALSE(id_factory.free_id(size));
    ASSERT_FALSE(id_factory.use_id(size));
    ASSERT_FALSE(id_factory.is_using(size));

    ASSERT_TRUE(id_factory.free_id(size - 1));
    ASSERT_TRUE(id_factory.use_id(size - 1));
    ASSERT_TRUE(id_factory.is_using(size - 1));
}

TEST(TestKBTree, SizeLarge) {
    uint32_t size = 1024 * 1024;

    std::cout << "test kupid::kbtree with size = " << size << '\n';

    kupid::kbtree id_factory{size};

    ASSERT_EQ(id_factory.size(), size);

    for (uint32_t i = 0; i < size; ++i) {
        id_factory.use_id(i);
    }

    auto mid = size / 2;

    id_factory.free_id(0);
    id_factory.free_id(mid);
    id_factory.free_id(size - 1);

    ASSERT_FALSE(id_factory.is_using(0));
    ASSERT_FALSE(id_factory.is_using(mid));
    ASSERT_FALSE(id_factory.is_using(size - 1));

    auto id = id_factory.get_id();
    std::cout << "#1. id = " << id << '\n';
    ASSERT_EQ(id, 0);

    id = id_factory.get_id();
    std::cout << "#2. id = " << id << '\n';

    ASSERT_EQ(id, mid);

    id = id_factory.get_id();
    std::cout << "#3. id = " << id << '\n';
    ASSERT_EQ(id, size - 1);

    ASSERT_TRUE(id_factory.is_using(0));
    ASSERT_TRUE(id_factory.is_using(mid));
    ASSERT_TRUE(id_factory.is_using(size - 1));

    id = id_factory.get_id();
    std::cout << "#4. id = " << id << '\n';
    ASSERT_EQ(id, -1);

    ASSERT_FALSE(id_factory.free_id(size));
    ASSERT_FALSE(id_factory.use_id(size));
    ASSERT_FALSE(id_factory.is_using(size));

    ASSERT_TRUE(id_factory.free_id(size - 1));
    ASSERT_TRUE(id_factory.use_id(size - 1));
    ASSERT_TRUE(id_factory.is_using(size - 1));
}

#ifdef TEST_XLARGE
TEST(TestKBTree, SizeXLarge) {
    uint32_t size = 16 * 1024 * 1024;

    std::cout << "test kupid::kbtree with size = " << size << '\n';

    kupid::kbtree id_factory{size};

    ASSERT_EQ(id_factory.size(), size);

    for (uint32_t i = 0; i < size; ++i) {
        id_factory.use_id(i);
    }

    auto mid = size / 2;

    id_factory.free_id(0);
    id_factory.free_id(mid);
    id_factory.free_id(size - 1);

    ASSERT_FALSE(id_factory.is_using(0));
    ASSERT_FALSE(id_factory.is_using(mid));
    ASSERT_FALSE(id_factory.is_using(size - 1));

    auto id = id_factory.get_id();
    std::cout << "#1. id = " << id << '\n';
    ASSERT_EQ(id, 0);

    id = id_factory.get_id();
    std::cout << "#2. id = " << id << '\n';

    ASSERT_EQ(id, mid);

    id = id_factory.get_id();
    std::cout << "#3. id = " << id << '\n';
    ASSERT_EQ(id, size - 1);

    ASSERT_TRUE(id_factory.is_using(0));
    ASSERT_TRUE(id_factory.is_using(mid));
    ASSERT_TRUE(id_factory.is_using(size - 1));

    id = id_factory.get_id();
    std::cout << "#4. id = " << id << '\n';
    ASSERT_EQ(id, -1);

    ASSERT_FALSE(id_factory.free_id(size));
    ASSERT_FALSE(id_factory.use_id(size));
    ASSERT_FALSE(id_factory.is_using(size));

    ASSERT_TRUE(id_factory.free_id(size - 1));
    ASSERT_TRUE(id_factory.use_id(size - 1));
    ASSERT_TRUE(id_factory.is_using(size - 1));
}
#endif

TEST(TestKBTree, RandomUnordered) {
    uint32_t size = 1024 * 1024;
    int rnd_size = 10;
    int j = 0;

    std::cout << "test kupid::kbtree with size = " << size << '\n';

    kupid::kbtree id_factory{size};
    kupid::krandom_int rnd_factory{size};

    ASSERT_EQ(id_factory.size(), size);

    for (uint32_t i = 0; i < size; ++i) {
        id_factory.use_id(i);
    }

    for (int i = 0; i < rnd_size; ++i) {
        auto rnd_num = rnd_factory.get_random();

        if (id_factory.is_using(rnd_num)) {
            ++j;
            id_factory.free_id(rnd_num);
            ASSERT_FALSE(id_factory.is_using(rnd_num));

            auto id = id_factory.get_id();
            std::cout << "#" << j << ". id = " << id << '\n';
            ASSERT_EQ(id, rnd_num);
        }
    }
}

TEST(TestKBTree, RandomOrdered) {
    uint32_t size = 1024 * 1024;
    int rnd_size = 10;
    std::set<uint32_t> rnd_set{};

    std::cout << "test kupid::kbtree with size = " << size << '\n';

    kupid::kbtree id_factory{size};
    kupid::krandom_int rnd_factory{size};

    ASSERT_EQ(id_factory.size(), size);

    for (uint32_t i = 0; i < size; ++i) {
        id_factory.use_id(i);
    }

    for (int i = 0; i < rnd_size; ++i) {
        auto rnd_num = rnd_factory.get_random();

        id_factory.free_id(rnd_num);
        ASSERT_FALSE(id_factory.is_using(rnd_num));

        rnd_set.insert(rnd_num);
    }

    for (int i = 0; i < rnd_size; ++i) {
        auto id = id_factory.get_id();
        std::cout << "#" << (i + 1) << ". id = " << id << '\n';

        auto it = rnd_set.find(id);
        ASSERT_NE(it, rnd_set.end());
    }
}
