#include <iostream>
#include <iomanip>
#include <bitset>

#if __cplusplus > 201703L  // C++20
#include <bit>
#endif

// g++ -std=c++14 bitops.cpp -o bitops

#define OFFSET_1        "                           " << std::setfill(' ') << std::setw(20)
#define ENDIANNESS      " HIGHEST ADDRESS                                                              LOWEST ADDRESS\n"\
                        "---------------+------------------------ LITTLE ENDIAN -----------------------+----------------\n"
#define MOST_SGN        "MOST SIGNIFICANT\n"\
                        "---------------+--------------------------------------------------------------+----------------\n"\
                        "               +      1       2       3       4       5       6       7       8\n"
#define OFFSET_2        "               "
#define LEAST_SGN       "               8       7       6       5       4       3       2       1      +\n"\
                        "---------------+--------------------------------------------------------------+----------------\n"\
                        "                                                                              LEAST SIGNIFICANT\n"
#define HEX_FORMAT      std::setfill('0') << std::setw(16) << std::uppercase << std::hex
#define DEC_FORMAT      std::dec

int main(int argc, char** argv) {
    uint64_t num64 = 0x0F0F0F0F0F0F0F0F;

    if (argc >= 2) {
        std::string arg{argv[1]};

        if (arg == "-h" || arg == "--help" || arg == "help") {
            std::cout << "usage:\n";
            std::cout << "\t" << argv[0] << " <number>\n";
            std::cout << "\tdefault number = " << num64 << '\n';
            std::cout << "\tapplies and displays bit ops on the number\n";
            return 0;
        }

        try {
            num64 = std::stoull(argv[1]);
        } catch (std::invalid_argument const &e) {
            std::cerr << "ERROR: bad input - " << argv[1] << " | " << e.what() << "\n\n";
            return 1;
        } catch (std::out_of_range const &e) {
            std::cerr << "ERROR: integer overflow - " << argv[1] << " | " << e.what() << "\n\n";
            return 2;
        }
    }

    std::string bit_op;

    std::bitset<64> bits(num64);
    std::bitset<64> comp_bits(~num64);

    std::cout << OFFSET_1 << num64 << " = 0x" << HEX_FORMAT << num64 << "\n\n";
    std::cout << ENDIANNESS;
    std::cout << MOST_SGN;
    std::cout << OFFSET_2 << bits << " :  bits\n"
              << OFFSET_2 << comp_bits << " : ~bits\n";
    std::cout << LEAST_SGN;

#if __cplusplus > 201703L  // C++20
    int popcount = std::popcount(num64);
    bit_op = "std::popcount";
    std::cout << '\n' << bit_op << "(x) : returns the number of 1 bits in the value of x\n";
    std::cout << '\t' << bit_op << "(0x" << HEX_FORMAT << num64 << ") = " << DEC_FORMAT << popcount << '\n';

    int countl_zero = std::countl_zero(num64);
    bit_op = "std::countl_zero";
    std::cout << '\n' << bit_op << "(x) : returns the number of consecutive 0 bits in the value of x, starting from the most significant bit ('left')\n";
    std::cout << '\t' << bit_op << "(0x" << HEX_FORMAT << num64 << ") = " << DEC_FORMAT << countl_zero << '\n';

    int countr_zero = std::countr_zero(num64);
    bit_op = "std::countr_zero";
    std::cout << '\n' << bit_op << "(x) : returns the number of consecutive 0 bits in the value of x, starting from the least significant bit ('right')\n";
    std::cout << '\t' << bit_op << "(0x" << HEX_FORMAT << num64 << ") = " << DEC_FORMAT << countr_zero << '\n';

    int countl_one = std::countl_one(num64);
    bit_op = "std::countl_one";
    std::cout << '\n' << bit_op << "(x) : returns the number of consecutive 1 bits in the value of x, starting from the most significant bit ('left')\n";
    std::cout << '\t' << bit_op << "(0x" << HEX_FORMAT << num64 << ") = " << DEC_FORMAT << countl_one << '\n';

    int countr_one = std::countr_one(num64);
    bit_op = "std::countr_one";
    std::cout << "\nstd::countr_one(x) : returns the number of consecutive 1 bits in the value of x, starting from the least significant bit ('right')\n";
    std::cout << '\t' << bit_op << "(0x" << HEX_FORMAT << num64 << ") = " << DEC_FORMAT << countr_one << '\n';
#else
    int popcount = __builtin_popcountll(num64);
    bit_op = "__builtin_popcountll";
    std::cout << '\n' << bit_op << "(x) : returns the number of 1-bits in x\n";
    std::cout << '\t' << bit_op << "(0x" << HEX_FORMAT << num64 << ") = " << DEC_FORMAT << popcount << '\n';

    int clz = __builtin_clzll(num64);
    bit_op = "__builtin_clzll";
    std::cout << '\n' << bit_op << "(x) : returns the number of leading 0-bits in x, starting at the most significant bit position\n";
    std::cout << "warning - if x is 0, the result is undefined\n";
    std::cout << '\t' << bit_op << "(0x" << HEX_FORMAT << num64 << ") = " << DEC_FORMAT << clz << '\n';

    int ctz = __builtin_ctzll(num64);
    bit_op = "__builtin_ctzll";
    std::cout << '\n' << bit_op << "(x) : returns the number of trailing 0-bits in x, starting at the least significant bit position\n";
    std::cout << "warning - if x is 0, the result is undefined\n";
    std::cout << '\t' << bit_op << "(0x" << HEX_FORMAT << num64 << ") = " << DEC_FORMAT << ctz << '\n';

    int ffs = __builtin_ffsll(num64);
    bit_op = "__builtin_ffsll";
    std::cout << '\n' << bit_op << "(x) : returns one plus the index of the least significant 1-bit of x, or if x is zero, returns zero\n";
    std::cout << '\t' << bit_op << "(0x" << HEX_FORMAT << num64 << ") = " << DEC_FORMAT << ffs << '\n';

    int ffs2 = __builtin_ffsll(~num64) - 1;
    bit_op = "__builtin_ffsll";
    std::cout << "\n" << bit_op << "(~x) - 1 : returns position of the first zero bit in x\n";
    auto comp_num64 = ~num64;
    std::cout << '\t' << bit_op << "(0x" << HEX_FORMAT << comp_num64 << ") - 1 = " << DEC_FORMAT << ffs2 << "\n\n";
#endif
}
