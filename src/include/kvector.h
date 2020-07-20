#ifndef KVECTOR_H
#define KVECTOR_H

#include <vector>
#include <cstdint>

namespace kupid {
    /**
     * std::vector
     * see:
     *      https://en.cppreference.com/w/cpp/container/vector
     */

    class kvector {
        public:
            kvector(uint32_t size)
                : _size{size}
            {
                _data.reserve(size);
            }

            kvector() = delete;

            int64_t next(bool is_using = true) {
                for (uint32_t id = 0 ; id < _size; ++id) {
                    if (_data[id] == false) {
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
                    _data[id] = state;
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
                    return _data[id] == true;
                } else {
                    return false;
                }
            }

            void clear() {
                _data.clear();
                _data.resize(_size);
            }

            uint32_t size() const {
                return _size;
            }

        private:
            uint32_t _size;
            std::vector<bool> _data;
    };
}

#endif // KVECTOR_H
