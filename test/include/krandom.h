#include <random>
#include <chrono>

namespace kupid {
    /**
    * std::mersenne_twister_engine
    * see:
    *       https://en.cppreference.com/w/cpp/numeric/random/mersenne_twister_engine
    *
    * std::uniform_int_distribution
    * see:
    *       https://en.cppreference.com/w/cpp/numeric/random/uniform_int_distribution
    *
    */

    class krandom_int {
        public:
            enum seed_type {
                seed_zero,
                seed_token,
                seed_epoch,
                seed_random
            };

            krandom_int(uint32_t size,
                        seed_type type = seed_type::seed_random,
                        uint32_t warm_up = 10000,
                        uint32_t seed = 787350) :
            _size{size},
            _type{type},
            _warm_up{warm_up} {
                switch (_type) {
                    case seed_type::seed_zero:
                        _seed = 0;
                        break;

                    case seed_type::seed_token:
                        _seed = seed;
                        break;

                    case seed_type::seed_epoch:
                        _seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
                        break;

                    case seed_type::seed_random:
                        _seed = std::random_device{}();
                        break;
                };

                _engine = std::mt19937_64{_seed};

                if (_warm_up > 0) {
                    _engine.discard(_warm_up);
                }

                _distribution = std::uniform_int_distribution<uint32_t>{0, _size - 1};
            }

            krandom_int() = delete;

            uint32_t size() {
                return _size;
            }

            seed_type type() {
                return _type;
            }

            uint32_t warm_up() {
                return _warm_up;
            }

            uint32_t seed() {
                return _seed;
            }

            uint32_t get_random() {
                return _distribution(_engine);
            }

        private:
            uint32_t _size;
            seed_type _type;
            uint32_t _warm_up;
            uint32_t _seed;
            std::mt19937_64 _engine;
            std::uniform_int_distribution<uint32_t> _distribution;
    };
}
