#include "gtest/gtest.h"
#include "../include/krandom.h"
#include "../../src/include/kbset.h"

TEST(TestKBSet, SizeZero) {
    uint32_t size = 0;

    std::cout << "test kupid::kbset with size = " << size << '\n';

    kupid::kbset<0> id_factory{};

    auto id = id_factory.get_id();
    std::cout << "#1. id = " << id << '\n';
    ASSERT_EQ(id, -1);
}

TEST(TestKBSet, SizeOne) {
    uint32_t size = 1;

    std::cout << "test kupid::kbset with size = " << size << '\n';

    kupid::kbset<1> id_factory{};

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

TEST(TestKBSet, SizeTwo) {
    uint32_t size = 2;

    std::cout << "test kupid::kbset with size = " << size << '\n';

    kupid::kbset<2> id_factory{};

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

TEST(TestKBSet, ClearUseHalf) {
    uint32_t size = 1 * 1024;

    std::cout << "test kupid::kbset with size = " << size << '\n';

    kupid::kbset<1 * 1024> id_factory{};

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

TEST(TestKBSet, SizeSmall) {
    uint32_t size = 1 * 1024;

    std::cout << "test kupid::kbset with size = " << size << '\n';

    kupid::kbset<1 * 1024> id_factory{};

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

TEST(TestKBSet, SizeMedium) {
    uint32_t size = 64 * 1024;

    std::cout << "test kupid::kbset with size = " << size << '\n';

    kupid::kbset<64 * 1024> id_factory{};

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

TEST(TestKBSet, SizeLarge) {
    uint32_t size = 1024 * 1024;

    std::cout << "test kupid::kbset with size = " << size << '\n';

    kupid::kbset<1024 * 1024> id_factory{};

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
TEST(TestKBSet, SizeXLarge) {
    uint32_t size = 16 * 1024 * 1024;

    std::cout << "test kupid::kbset with size = " << size << '\n';

    kupid::kbset<16* 1024 * 1024> id_factory{};

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

TEST(TestKBSet, RandomUnordered) {
    uint32_t size = 1024 * 1024;
    int rnd_size = 10;
    int j = 0;

    std::cout << "test kupid::kbset with size = " << size << '\n';

    kupid::kbset<1024 * 1024> id_factory{};
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

TEST(TestKBSet, RandomOrdered) {
    uint32_t size = 1024 * 1024;
    int rnd_size = 10;
    std::set<uint32_t> rnd_set{};

    std::cout << "test kupid::kbset with size = " << size << '\n';

    kupid::kbset<1024 * 1024>  id_factory{};
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
