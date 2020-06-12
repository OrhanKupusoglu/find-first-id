#include <iostream>

#include "../include/kbtree.h"
#include "../include/kbset.h"
#include "../include/kset.h"
#include "../include/kvector.h"

// g++ -std=c++14 -O3 main.cpp -o kupid

int main(int argc, char** argv) {
    constexpr const int bset_size = 8192;
    int read_size = 8192;

    if (argc >= 2) {
        std::string arg{argv[1]};

        if (arg == "-h" || arg == "--help" || arg == "help") {
            std::cout << "usage:\n";
            std::cout << "\t" << argv[0] << " <size>\n";
            std::cout << "\tdefault size = " << read_size << '\n';
            std::cout << "\tcreates instances of id factories\n";
            return 0;
        }

        try {
            read_size = std::stoi(argv[1]);
        } catch (std::invalid_argument const &e) {
            std::cerr << "ERROR: bad input - " << argv[1] << " | " << e.what() << "\n\n";
            return 1;
        } catch (std::out_of_range const &e) {
            std::cerr << "ERROR: integer overflow - " << argv[1] << " | " << e.what() << "\n\n";
            return 2;
        }
    }

    uint32_t size = read_size;
    int last = size - 1;
    std::string line_sep(60, '-');

    std::cout << "kupid::kbtree\n" << line_sep << '\n';
    {
        kupid::kbtree id_factory{size};

        std::cout << "++ size = " << size << " : all used\n";

        for (uint32_t i = 0; i < size; ++i) {
            id_factory.use_id(i);
        }

        std::cout << "++ last id is freed\n";
        id_factory.free_id(last);

        auto id = id_factory.get_id();
        std::cout << "get_id() = " << id << '\n';
        id = id_factory.get_id();
        std::cout << "get_id() = " << id << '\n';

        std::cout << "++ cleared\n";
        id_factory.clear();

        id = id_factory.get_id();
        std::cout << "get_id() = " << id << '\n';
    }

    std::cout << "\nkupid::kvector\n" << line_sep << '\n';
    {
        kupid::kvector id_factory{size};

        std::cout << "++ size = " << size << " : all used\n";

        for (uint32_t i = 0; i < size; ++i) {
            id_factory.use_id(i);
        }

        std::cout << "++ last id is freed\n";
        id_factory.free_id(last);

        auto id = id_factory.get_id();
        std::cout << "get_id() = " << id << '\n';
        id = id_factory.get_id();
        std::cout << "get_id() = " << id << '\n';

        std::cout << "++ cleared\n";
        id_factory.clear();

        id = id_factory.get_id();
        std::cout << "get_id() = " << id << '\n';
    }

    std::cout << "\nkupid::kbset - templated\n" << line_sep << '\n';
    {
        uint32_t size = bset_size;
        int last = size - 1;

        kupid::kbset<bset_size> id_factory{};

        std::cout << "++ size = " << size << " : all used\n";

        for (uint32_t i = 0; i < size; ++i) {
            id_factory.use_id(i);
        }

        std::cout << "++ last id is freed\n";
        id_factory.free_id(last);

        auto id = id_factory.get_id();
        std::cout << "get_id() = " << id << '\n';
        id = id_factory.get_id();
        std::cout << "get_id() = " << id << '\n';

        std::cout << "++ cleared\n";
        id_factory.clear();

        id = id_factory.get_id();
        std::cout << "get_id() = " << id << '\n';
    }

    std::cout << "\nkupid::kset\n" << line_sep << '\n';
    {
        kupid::kset id_factory{size};

        std::cout << "++ size = " << size << " : all used\n";

        for (uint32_t i = 0; i < size; ++i) {
            id_factory.use_id(i);
        }

        std::cout << "++ last id is freed\n";
        id_factory.free_id(last);

        auto id = id_factory.get_id();
        std::cout << "get_id() = " << id << '\n';
        id = id_factory.get_id();
        std::cout << "get_id() = " << id << '\n';

        std::cout << "++ cleared\n";
        id_factory.clear();

        id = id_factory.get_id();
        std::cout << "get_id() = " << id << '\n';
    }
}
