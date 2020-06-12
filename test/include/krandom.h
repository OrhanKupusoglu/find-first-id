#include <random>

namespace kupid {
    class krandom_int {
        public:
            krandom_int(uint32_t N)
                : _engine{std::random_device{}()},
                  _distribution{0, N - 1}
            {}

            krandom_int() = delete;

            uint32_t get_random() {
                return _distribution(_engine);
            }

        private:
            std::mt19937 _engine;
            std::uniform_int_distribution<uint32_t> _distribution;
    };
}
