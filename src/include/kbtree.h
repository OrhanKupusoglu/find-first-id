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
     *
     * De Bruijn sequence
     * see:
     *      https://en.wikipedia.org/wiki/De_Bruijn_sequence
     *
     * De Bruijn multiplication
     * see:
     *      https://www.chessprogramming.org/BitScan#DeBruijnMultiplation
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

                // max 6 data layers: 2^32 = (2^6)^5 x (2^2)
                _data.reserve(6);

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

                _data.shrink_to_fit();
            }

            kbtree() = delete;                                  // default constructor
            kbtree(const kbtree& copy) = delete;                // copy constructor
            kbtree& operator=(const kbtree& copy) = delete;     // copy assignment
            kbtree(kbtree&& move) = default;                    // move constructor
            kbtree& operator=(kbtree&& move) = default;         // move assignment

            int64_t next(bool is_using = true) {
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

            static inline bool is_bit_on(uint64_t bits, uint8_t i) {
                return (bits & get_on_64_bit(i)) > 0;
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
    #ifndef DE_BRUIJN_SEQUENCE
                return __builtin_popcountll(bits) == 64;
    #else
                return bits == 0xFFFFFFFFFFFFFFFF;
    #endif
#endif
            }

            static inline int32_t find_first_free_bit(uint64_t bits) {
#if __cplusplus > 201703L  // C++20
                return std::countr_one(bits);
#else
    #ifndef DE_BRUIJN_SEQUENCE
                return __builtin_ffsll(~bits) - 1;
    #else
                static constexpr std::array<uint8_t, 64> de_bruijn_64 = {
                     0,  1, 48,  2, 57, 49, 28,  3,
                    61, 58, 50, 42, 38, 29, 17,  4,
                    62, 55, 59, 36, 53, 51, 43, 22,
                    45, 39, 33, 30, 24, 18, 12,  5,
                    63, 47, 56, 27, 60, 41, 37, 16,
                    54, 35, 52, 21, 44, 32, 23, 11,
                    46, 26, 40, 15, 34, 20, 31, 10,
                    25, 14, 19,  9, 13,  8,  7,  6
                };

                static constexpr uint64_t de_bruijn_magic = 0x03F79D71B4CB0A89;

                bits  = ~bits;
                bits &= -bits;
                bits *=  de_bruijn_magic;
                uint8_t v = bits >> 58;

                return de_bruijn_64[v];
    #endif
#endif
            }

        private:
            uint32_t _size;
            uint32_t _slice = 0;  // initial value
            std::vector<std::unique_ptr<uint64_t[]>> _data;

        private:
            static uint64_t get_on_64_bit(uint8_t i) {
                // { 1UL << i, i = [0, 64) }
                static constexpr std::array<uint64_t, 64> on_64 = {
                    1UL << 0,  1UL << 1,  1UL << 2,  1UL << 3,
                    1UL << 4,  1UL << 5,  1UL << 6,  1UL << 7,
                    1UL << 8,  1UL << 9,  1UL << 10, 1UL << 11,
                    1UL << 12, 1UL << 13, 1UL << 14, 1UL << 15,
                    1UL << 16, 1UL << 17, 1UL << 18, 1UL << 19,
                    1UL << 20, 1UL << 21, 1UL << 22, 1UL << 23,
                    1UL << 24, 1UL << 25, 1UL << 26, 1UL << 27,
                    1UL << 28, 1UL << 29, 1UL << 30, 1UL << 31,
                    1UL << 32, 1UL << 33, 1UL << 34, 1UL << 35,
                    1UL << 36, 1UL << 37, 1UL << 38, 1UL << 39,
                    1UL << 40, 1UL << 41, 1UL << 42, 1UL << 43,
                    1UL << 44, 1UL << 45, 1UL << 46, 1UL << 47,
                    1UL << 48, 1UL << 49, 1UL << 50, 1UL << 51,
                    1UL << 52, 1UL << 53, 1UL << 54, 1UL << 55,
                    1UL << 56, 1UL << 57, 1UL << 58, 1UL << 59,
                    1UL << 60, 1UL << 61, 1UL << 62, 1UL << 63
                };

                return on_64[i];
            }

            static uint64_t get_off_64_bit(uint8_t i) {
                // { ~(1UL << i), i = [0, 64) }
                static constexpr std::array<uint64_t, 64> off_64 = {
                    ~(1UL << 0),  ~(1UL << 1),  ~(1UL << 2),  ~(1UL << 3),
                    ~(1UL << 4),  ~(1UL << 5),  ~(1UL << 6),  ~(1UL << 7),
                    ~(1UL << 8),  ~(1UL << 9),  ~(1UL << 10), ~(1UL << 11),
                    ~(1UL << 12), ~(1UL << 13), ~(1UL << 14), ~(1UL << 15),
                    ~(1UL << 16), ~(1UL << 17), ~(1UL << 18), ~(1UL << 19),
                    ~(1UL << 20), ~(1UL << 21), ~(1UL << 22), ~(1UL << 23),
                    ~(1UL << 24), ~(1UL << 25), ~(1UL << 26), ~(1UL << 27),
                    ~(1UL << 28), ~(1UL << 29), ~(1UL << 30), ~(1UL << 31),
                    ~(1UL << 32), ~(1UL << 33), ~(1UL << 34), ~(1UL << 35),
                    ~(1UL << 36), ~(1UL << 37), ~(1UL << 38), ~(1UL << 39),
                    ~(1UL << 40), ~(1UL << 41), ~(1UL << 42), ~(1UL << 43),
                    ~(1UL << 44), ~(1UL << 45), ~(1UL << 46), ~(1UL << 47),
                    ~(1UL << 48), ~(1UL << 49), ~(1UL << 50), ~(1UL << 51),
                    ~(1UL << 52), ~(1UL << 53), ~(1UL << 54), ~(1UL << 55),
                    ~(1UL << 56), ~(1UL << 57), ~(1UL << 58), ~(1UL << 59),
                    ~(1UL << 60), ~(1UL << 61), ~(1UL << 62), ~(1UL << 63)
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
