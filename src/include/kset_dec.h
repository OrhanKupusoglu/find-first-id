#ifndef KSET_DECREASING_H
#define KSET_DECREASING_H

#include <set>
#include <algorithm>
#include <cstdint>

namespace kupid {
    /**
     * keep track of available IDs
     * start with all IDs as available, and remove used IDs: set is decreasing
     *
     * std::set
     * see:
     *      https://en.cppreference.com/w/cpp/container/set
     */

    class kset_dec {
        public:
            kset_dec(uint32_t size)
                : _size{size}
            {
                clear();
            }

            kset_dec() = delete;

            int64_t get_id(bool is_using = true) {
                auto it = _data.begin();

                if  (it == _data.end()) {
                    return -1;
                } else {
                    if (is_using) {
                        _data.erase(it);
                    }
                }

                return  *it;
            }

            bool use_id(uint32_t id) {
                if (id < _size) {
                    auto it = _data.find(id);
                    auto found = it != _data.end();
                    if (found) {
                        _data.erase(it);
                    }
                    return found;
                } else {
                    return false;
                }
            }

            bool free_id(uint32_t id) {
                if (id < _size) {
                    auto result = _data.insert(id);
                    return result.second;
                } else {
                    return false;
                }
            }

            bool is_using(uint32_t id) const {
                if (id < _size) {
                    auto it = _data.find(id);
                    return it == _data.end();
                } else {
                    return false;
                }
            }

            void clear() {
                _data.clear();
                // start with all IDs are available
                for (uint32_t id = 0; id < _size; ++id) {
                    _data.insert(id);
                }
            }

            uint32_t size() const {
                return _size;
            }

            uint32_t data_size() const {
                return _data.size();
            }

        private:
            uint32_t _size;
            std::set<uint32_t> _data{};
    };
}

#endif // KSET_H
