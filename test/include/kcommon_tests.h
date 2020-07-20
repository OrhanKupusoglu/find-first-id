#include "gtest/gtest.h"
#include "../include/krandom.h"
#include "../../src/include/kbset.h"

template<typename T>
class kcommon_tests {
    public:
        kcommon_tests(std::string name) : _name{name} {};

        kcommon_tests() = delete;

        void test_size_zero() {
            uint32_t size = 0;

            std::cout << "test " << _name << " with size = " << size << '\n';

            T id_factory = T{size};

            auto id = id_factory.next();
            std::cout << "#1. id = " << id << '\n';
            ASSERT_EQ(id, -1);
        }

        void test_size_one() {
            uint32_t size = 1;

            std::cout << "test " << _name << " with size = " << size << '\n';

            T id_factory = T{size};

            auto id = id_factory.next(false);
            std::cout << "#1. id = " << id << " - not marked as used\n";
            ASSERT_EQ(id, 0);

            id = id_factory.next();
            std::cout << "#1. id = " << id << '\n';
            ASSERT_EQ(id, 0);

            id = id_factory.next();
            std::cout << "#2. id = " << id << '\n';
            ASSERT_EQ(id, -1);
        }

        void test_size_two() {
            uint32_t size = 2;

            std::cout << "test " << _name << " with size = " << size << '\n';

            T id_factory = T{size};

            auto id = id_factory.next();
            std::cout << "#1. id = " << id << '\n';
            ASSERT_EQ(id, 0);

            id = id_factory.next();
            std::cout << "#2. id = " << id << '\n';
            ASSERT_EQ(id, 1);

            id = id_factory.next();
            std::cout << "#3. id = " << id << '\n';
            ASSERT_EQ(id, -1);
        }

        void test_clear_use_half() {
            uint32_t size = 1 * 1024;

            std::cout << "test " << _name << " with size = " << size << '\n';

            T id_factory = T{size};

            ASSERT_EQ(id_factory.size(), size);

            for (uint32_t i = 0; i < size; ++i) {
                id_factory.use_id(i);
            }

            std::cout << "id_factory is full\n";

            auto id = id_factory.next();
            std::cout << "#1. id = " << id << '\n';
            ASSERT_EQ(id, -1);

            id_factory.clear();

            std::cout << "id_factory is cleared\n";

            id = id_factory.next();
            std::cout << "#2. id = " << id << '\n';
            ASSERT_EQ(id, 0);

            auto mid = size / 2;

            for (uint32_t i = 0; i < mid; ++i) {
                id_factory.use_id(i);
            }

            std::cout << "id_factory is 1/2 used\n";

            id = id_factory.next();
            std::cout << "#3. id = " << id << '\n';
            ASSERT_EQ(id, mid);
        }

        void test_size_small() {
            uint32_t size = 1 * 1024;

            std::cout << "test " << _name << " with size = " << size << '\n';

            T id_factory = T{size};

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

            auto id = id_factory.next();
            std::cout << "#1. id = " << id << '\n';
            ASSERT_EQ(id, 0);

            id = id_factory.next();
            std::cout << "#2. id = " << id << '\n';
            ASSERT_EQ(id, mid);

            id = id_factory.next();
            std::cout << "#3. id = " << id << '\n';
            ASSERT_EQ(id, size - 1);

            ASSERT_TRUE(id_factory.is_using(0));
            ASSERT_TRUE(id_factory.is_using(mid));
            ASSERT_TRUE(id_factory.is_using(size - 1));

            id = id_factory.next();
            std::cout << "#4. id = " << id << '\n';
            ASSERT_EQ(id, -1);

            ASSERT_FALSE(id_factory.free_id(size));
            ASSERT_FALSE(id_factory.use_id(size));
            ASSERT_FALSE(id_factory.is_using(size));

            ASSERT_TRUE(id_factory.free_id(size - 1));
            ASSERT_TRUE(id_factory.use_id(size - 1));
            ASSERT_TRUE(id_factory.is_using(size - 1));
        }

        void test_size_medium() {
            uint32_t size = 64 * 1024;

            std::cout << "test " << _name << " with size = " << size << '\n';

            T id_factory = T{size};

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

            auto id = id_factory.next();
            std::cout << "#1. id = " << id << '\n';
            ASSERT_EQ(id, 0);

            id = id_factory.next();
            std::cout << "#2. id = " << id << '\n';
            ASSERT_EQ(id, mid);

            id = id_factory.next();
            std::cout << "#3. id = " << id << '\n';
            ASSERT_EQ(id, size - 1);

            ASSERT_TRUE(id_factory.is_using(0));
            ASSERT_TRUE(id_factory.is_using(mid));
            ASSERT_TRUE(id_factory.is_using(size - 1));

            id = id_factory.next();
            std::cout << "#4. id = " << id << '\n';
            ASSERT_EQ(id, -1);

            ASSERT_FALSE(id_factory.free_id(size));
            ASSERT_FALSE(id_factory.use_id(size));
            ASSERT_FALSE(id_factory.is_using(size));

            ASSERT_TRUE(id_factory.free_id(size - 1));
            ASSERT_TRUE(id_factory.use_id(size - 1));
            ASSERT_TRUE(id_factory.is_using(size - 1));
        }

        void test_size_large() {
            uint32_t size = 1024 * 1024;

            std::cout << "test " << _name << " with size = " << size << '\n';

            T id_factory = T{size};

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

            auto id = id_factory.next();
            std::cout << "#1. id = " << id << '\n';
            ASSERT_EQ(id, 0);

            id = id_factory.next();
            std::cout << "#2. id = " << id << '\n';

            ASSERT_EQ(id, mid);

            id = id_factory.next();
            std::cout << "#3. id = " << id << '\n';
            ASSERT_EQ(id, size - 1);

            ASSERT_TRUE(id_factory.is_using(0));
            ASSERT_TRUE(id_factory.is_using(mid));
            ASSERT_TRUE(id_factory.is_using(size - 1));

            id = id_factory.next();
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
        void test_size_xlarge() {
            uint32_t size = 16 * 1024 * 1024;

            std::cout << "test " << _name << " with size = " << size << '\n';

            T id_factory = T{size};

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

            auto id = id_factory.next();
            std::cout << "#1. id = " << id << '\n';
            ASSERT_EQ(id, 0);

            id = id_factory.next();
            std::cout << "#2. id = " << id << '\n';

            ASSERT_EQ(id, mid);

            id = id_factory.next();
            std::cout << "#3. id = " << id << '\n';
            ASSERT_EQ(id, size - 1);

            ASSERT_TRUE(id_factory.is_using(0));
            ASSERT_TRUE(id_factory.is_using(mid));
            ASSERT_TRUE(id_factory.is_using(size - 1));

            id = id_factory.next();
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

        void test_random_unordered() {
            uint32_t size = 1024 * 1024;
            int rnd_size = 10;
            int j = 0;

            std::cout << "test " << _name << " with size = " << size << '\n';

            T id_factory = T{size};
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

                    auto id = id_factory.next();
                    std::cout << "#" << j << ". id = " << id << '\n';
                    ASSERT_EQ(id, rnd_num);
                }
            }
        }

        void test_random_ordered() {
            uint32_t size = 1024 * 1024;
            int rnd_size = 10;
            std::set<uint32_t> rnd_set{};

            std::cout << "test " << _name << " with size = " << size << '\n';

            T id_factory = T{size};
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
                auto id = id_factory.next();
                std::cout << "#" << (i + 1) << ". id = " << id << '\n';

                auto it = rnd_set.find(id);
                ASSERT_NE(it, rnd_set.end());
            }
        }

    private:
        std::string _name;
};
