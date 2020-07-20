#ifndef KSET_INCREASING_H
#define KSET_INCREASING_H

#include <set>
#include <algorithm>
#include <cstdint>

namespace kupid {
    /**
     * keep track of used IDs
     * start with none used, and add used IDs: set is increasing
     *
     * std::set
     * see:
     *      https://en.cppreference.com/w/cpp/container/set
     */

    class kset_inc {
        public:
            kset_inc(uint32_t size)
                : _size{size}
            {}

            kset_inc() = delete;

            int64_t next(bool is_using = true) {
                uint32_t id;

                if (_size == 0) {
                    return -1;
                } else if (_data.size() == _size) {
                    return -1;
                } else if (_data.empty()) {
                    id = 0;
                } else {
                    // check extremes first
                    auto result = std::minmax_element(_data.begin(), _data.end());
                    auto id_min = *(result.first);
                    auto id_max = *(result.second);

                    if (id_min > 0) {
                        id = id_min - 1;
                    } else {
                        auto it = std::adjacent_find(_data.begin(),
                                                     _data.end(),
                                                     [] (uint32_t a, uint32_t b) { return b - a > 1; });
                        if (it == _data.end()) {
                            id = id_max + 1;
                        } else { // in-between
                            id = *it + 1;
                        }
                    }
                }

                if (is_using) {
                    use_id(id);
                }

                return id;
            }

            bool use_id(uint32_t id) {
                if (id < _size) {
                    auto result = _data.insert(id);
                    return result.second;
                } else {
                    return false;
                }
            }

            bool free_id(uint32_t id) {
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

            bool is_using(uint32_t id) const {
                if (id < _size) {
                    auto it = _data.find(id);
                    return it != _data.end();
                } else {
                    return false;
                }
            }

            void clear() {
                _data.clear();
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
