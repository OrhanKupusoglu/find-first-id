#ifndef KBTREE_H
#define KBTREE_H

#include <vector>
#include <memory>
#include <cstring>

#if __cplusplus > 201703L  // C++20
#include <bit>
#endif

namespace kupid {
    /**
     * Find first set
     * see:
     *      https://en.wikipedia.org/wiki/Find_first_set
     *
     * GNU
     * 6.59 Other Built-in Functions Provided by GCC
     * see:
     *      https://gcc.gnu.org/onlinedocs/gcc/Other-Builtins.html
     */

    class kbtree {
        public:
            struct div_mod {
                uint32_t div;
                uint32_t mod;
            };

            kbtree(uint32_t size)
                : _size{size}
            {
                uint32_t slice = size;
                div_mod dm;

                do {
                    dm = get_div_and_mod_by_64(slice);
                    slice = get_div_or_plus_1(dm);

                    if (_slice == 0) {
                        _slice = slice;
                    }

#if __cplusplus == 201103L  // C++11
                    _data.push_back(std::unique_ptr<uint64_t[]>(new uint64_t[slice]()));
#else
                    _data.push_back(std::make_unique<uint64_t[]>(slice));
#endif
                } while (dm.div > 0);
            }

            kbtree() = delete;

            int64_t get_id(bool is_using = true) {
                uint32_t rank = 0;

                for (auto it = _data.rbegin(); it != _data.rend(); ++it) {
                    uint64_t data = (*it)[rank];
                    int32_t offset = find_first_free_bit(data);

                    if (offset < 0) {
                        return -1;
                    }

                    rank *= 64;
                    rank += offset;
                }

                if (rank >= _size) {
                    return -1;
                }

                if (is_using) {
                    use_id(rank);
                }

                return rank;
            }

            bool use_id(uint32_t id) {
                return set_id_state(id, true);
            }

            bool free_id(uint32_t id) {
                return set_id_state(id, false);
            }


            bool is_using(uint32_t id) const {
                if (id < _size) {
                    div_mod id_dm = get_div_and_mod_by_64(id);
                    return is_bit_on(_data[0][id_dm.div], id_dm.mod);
                } else {
                    return false;
                }
            }

            void clear() {
                uint32_t slice = _slice;
                for (auto it = _data.begin(); it != _data.end(); ++it) {
                    auto arr = (*it).get();

                    std::memset(arr, 0, slice * sizeof(arr[0]));

                    div_mod dm = get_div_and_mod_by_64(slice);
                    slice = get_div_or_plus_1(dm);
                }
            }

            uint32_t size() const {
                return _size;
            }

            uint32_t slice() const {
                return _slice;
            }

        // public only for unit tests
        public:
            int64_t get_data(uint32_t id, bool is_index = true) const {
                if (is_index) {
                    if (id < _slice) {
                        return _data[0][id];
                    } else {
                        return -2;
                    }
                } else {
                    if (id < _size) {
                        div_mod id_dm = get_div_and_mod_by_64(id);
                        return _data[0][id_dm.div];
                    } else {
                        return -4;
                    }
                }
            }

            // 64 = 2**6, 63 = 64 - 1
            static inline div_mod get_div_and_mod_by_64(uint32_t bits) {
#if __cplusplus > 201703L  // C++20
                return div_mod{ .div = bits >> 6,
                                .mod = bits & 63 };
#else
                div_mod dm{};
                dm.div = bits >> 6;
                dm.mod = bits & 63;
                return dm;
#endif
            }

            static inline uint32_t get_div_or_plus_1(div_mod& dm) {
                return dm.mod > 0 ? dm.div + 1 : dm.div;
            }

            static inline bool is_bit_on(uint64_t bits, uint64_t i) {
                return (bits & (1L << i)) > 0;
            }

            static inline void set_bit_on(uint64_t& bits, uint8_t i) {
                bits |= get_on_64_bit(i);
            }

            static inline void set_bit_off(uint64_t& bits, uint8_t i) {
                bits &= get_off_64_bit(i);
            }

            static inline void set_bit(uint64_t& bits, uint8_t i, bool on) {
                on ? set_bit_on(bits, i) : set_bit_off(bits, i);
            }

            static inline bool is_full(uint64_t bits) {
#if __cplusplus > 201703L  // C++20
                return std::popcount(bits) == 64;
#else
                return __builtin_popcountll(bits) == 64;
#endif
            }

            static inline int32_t find_first_free_bit(uint64_t bits) {
#if __cplusplus > 201703L  // C++20
                return std::countr_one(bits);
#else
                return __builtin_ffsll(~bits) - 1;
#endif
            }

        private:
            uint32_t _size;
            uint32_t _slice = 0;  // initial value
            std::vector<std::unique_ptr<uint64_t[]>> _data;

        private:
            static uint64_t get_on_64_bit(uint8_t i) {
                // { 1UL << i, i = [0, 64) }
                static const std::array<uint64_t, 64> on_64 = {
                    0x0000000000000001, 0x0000000000000002, 0x0000000000000004, 0x0000000000000008,
                    0x0000000000000010, 0x0000000000000020, 0x0000000000000040, 0x0000000000000080,
                    0x0000000000000100, 0x0000000000000200, 0x0000000000000400, 0x0000000000000800,
                    0x0000000000001000, 0x0000000000002000, 0x0000000000004000, 0x0000000000008000,
                    0x0000000000010000, 0x0000000000020000, 0x0000000000040000, 0x0000000000080000,
                    0x0000000000100000, 0x0000000000200000, 0x0000000000400000, 0x0000000000800000,
                    0x0000000001000000, 0x0000000002000000, 0x0000000004000000, 0x0000000008000000,
                    0x0000000010000000, 0x0000000020000000, 0x0000000040000000, 0x0000000080000000,
                    0x0000000100000000, 0x0000000200000000, 0x0000000400000000, 0x0000000800000000,
                    0x0000001000000000, 0x0000002000000000, 0x0000004000000000, 0x0000008000000000,
                    0x0000010000000000, 0x0000020000000000, 0x0000040000000000, 0x0000080000000000,
                    0x0000100000000000, 0x0000200000000000, 0x0000400000000000, 0x0000800000000000,
                    0x0001000000000000, 0x0002000000000000, 0x0004000000000000, 0x0008000000000000,
                    0x0010000000000000, 0x0020000000000000, 0x0040000000000000, 0x0080000000000000,
                    0x0100000000000000, 0x0200000000000000, 0x0400000000000000, 0x0800000000000000,
                    0x1000000000000000, 0x2000000000000000, 0x4000000000000000, 0x8000000000000000
                };

                return on_64[i];
            }

            static uint64_t get_off_64_bit(uint8_t i) {
                // { ~(1UL << i), i = [0, 64) }
                static const std::array<uint64_t, 64> off_64 = {
                    0xfffffffffffffffe, 0xfffffffffffffffd, 0xfffffffffffffffb, 0xfffffffffffffff7,
                    0xffffffffffffffef, 0xffffffffffffffdf, 0xffffffffffffffbf, 0xffffffffffffff7f,
                    0xfffffffffffffeff, 0xfffffffffffffdff, 0xfffffffffffffbff, 0xfffffffffffff7ff,
                    0xffffffffffffefff, 0xffffffffffffdfff, 0xffffffffffffbfff, 0xffffffffffff7fff,
                    0xfffffffffffeffff, 0xfffffffffffdffff, 0xfffffffffffbffff, 0xfffffffffff7ffff,
                    0xffffffffffefffff, 0xffffffffffdfffff, 0xffffffffffbfffff, 0xffffffffff7fffff,
                    0xfffffffffeffffff, 0xfffffffffdffffff, 0xfffffffffbffffff, 0xfffffffff7ffffff,
                    0xffffffffefffffff, 0xffffffffdfffffff, 0xffffffffbfffffff, 0xffffffff7fffffff,
                    0xfffffffeffffffff, 0xfffffffdffffffff, 0xfffffffbffffffff, 0xfffffff7ffffffff,
                    0xffffffefffffffff, 0xffffffdfffffffff, 0xffffffbfffffffff, 0xffffff7fffffffff,
                    0xfffffeffffffffff, 0xfffffdffffffffff, 0xfffffbffffffffff, 0xfffff7ffffffffff,
                    0xffffefffffffffff, 0xffffdfffffffffff, 0xffffbfffffffffff, 0xffff7fffffffffff,
                    0xfffeffffffffffff, 0xfffdffffffffffff, 0xfffbffffffffffff, 0xfff7ffffffffffff,
                    0xffefffffffffffff, 0xffdfffffffffffff, 0xffbfffffffffffff, 0xff7fffffffffffff,
                    0xfeffffffffffffff, 0xfdffffffffffffff, 0xfbffffffffffffff, 0xf7ffffffffffffff,
                    0xefffffffffffffff, 0xdfffffffffffffff, 0xbfffffffffffffff, 0x7fffffffffffffff
                };

                return off_64[i];
            }

            bool set_id_state(uint32_t index, bool state) const {
                if (index < _size) {
                    uint32_t val = index;
                    div_mod index_dm;

                    // start from the data layer (first layer)
                    for (auto it = _data.begin(); it != _data.end(); ++it) {
                        index_dm = get_div_and_mod_by_64(val);
                        uint64_t& data = (*it)[index_dm.div];
                        set_bit(data, index_dm.mod, state);

                        // if removed and made available or 64 bits are fully used, mark this on the next level
                        if (!state || is_full(data)) {
                            val = index_dm.div;
                        } else {
                            break;
                        }
                    }

                    return true;
                } else {
                    return false;
                }
            }
    };
}

#endif // KBTREE_H
