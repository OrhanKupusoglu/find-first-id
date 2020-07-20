#ifndef KBSET_H
#define KBSET_H

#include <bitset>

namespace kupid {
    /**
     * std::bitset
     * see:
     *      https://en.cppreference.com/w/cpp/utility/bitset
     */

    template<size_t N>
    class kbset {
        public:
            kbset() = default;

            int next(bool is_using = true) {
                for (uint32_t id = 0 ; id < _size; ++id) {
                    if (_data.test(id) == false) {
                        if (is_using) {
                            use_id(id);
                        }
                        return id;
                    }
                }

                return -1;
            }

            bool set_id_state(uint32_t id, bool state) {
                if (id < _size) {
                    _data.set(id, state);
                    return true;
                } else {
                    return false;
                }
            }

            bool use_id(uint32_t id) {
                return set_id_state(id, true);
            }

            bool free_id(uint32_t id) {
                return set_id_state(id, false);
            }

            bool is_using(uint32_t id) const {
                if (id < _size) {
                    return _data.test(id);
                } else {
                    return false;
                }
            }

            void clear() {
                _data.reset();
            }

            uint32_t size() const {
                return _size;
            }

        private:
            uint32_t _size{N};
            std::bitset<N> _data{};
    };
}

#endif // KBSET_H
