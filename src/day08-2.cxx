#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <algorithm>
#include <cstdint>
#include <map>

// The 10 bit patterns corresponding to valid combinations of lit segments
//  0000
// 1    2
// 1    2
//  3333
// 4    5
// 4    5
//  6666
std::map<std::uint8_t, bool> const patterns{
    {0b01110111, false}, // 0
    {0b00100100, false}, // 1
    {0b01011101, false}, // 2
    {0b01101101, false}, // 3
    {0b00101110, false}, // 4
    {0b01101011, false}, // 5
    {0b01111011, false}, // 6
    {0b00100101, false}, // 7
    {0b01111111, false}, // 8
    {0b01101111, false}, // 9
};

// A map of strings of bit positions to their numeric values. Assume
// bit positions are sorted. (e.g. 012456 -> 0)
std::map<std::string, int> const values{
    {"012456", 0},
    {"25", 1},
    {"02346", 2},
    {"02356", 3},
    {"1235", 4},
    {"01356", 5},
    {"013456", 6},
    {"025", 7},
    {"0123456", 8},
    {"012356", 9},
};

int main(int argc, char const * const argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <filename>\n";
        return 1;
    }

    std::ifstream fstr;
    fstr.exceptions(std::ios_base::badbit | std::ios_base::failbit);
    fstr.open(argv[1]);

    std::istringstream istr;
    istr.exceptions(std::ios_base::badbit | std::ios_base::failbit);

    int count = 0;
    for (std::string line; (!fstr.eof())
            && (fstr.peek() != std::ifstream::traits_type::eof())
            && std::getline(fstr, line); )
    {
        istr.clear();
        istr.str(line);

        // Grab each of the 10 unique signal patterns.
        std::string combinations[10];
        for (auto & c : combinations)
        {
            istr >> c;
            istr.ignore();
        }

        // Iterate through all permutations of [a, g], where order indicates
        // bit assignment. For each assignment, check whether interpreting the
        // 10 input signals according to that assignment generates only valid
        // digits.
        std::string order("abcdefg");
        std::map<char, int> bits;
        while (true)
        {
            // Map each letter to its bit index
            bits.clear();
            int i = 0;
            for (auto j : order)
            {
                bits[j] = i++;
            }

            // Test each of the ten combinations for validity
            auto found{patterns};
            for (auto & c : combinations)
            {
                std::uint8_t p = 0;
                for (auto j : c)
                {
                    p += 1 << bits[j];
                }
                found[p] = true;
            }
            bool allFound = true;
            for (auto f : found)
            {
                if (!f.second)
                {
                    allFound = false;
                    break;
                }
            }
            if (allFound)
                break;

            // Not all ten signals were valid with the current interpretation
            // of letters as segments
            if (!std::next_permutation(order.begin(), order.end()))
            {
                std::cerr << "Tried all permutations!\n";
                return 1;
            }
        }

        // Ignore pipe character
        istr.ignore();

        // Grab the four signals corresponding to the displayed digits,
        // ignoring leading space. Replacing [a, g] with [0, 6] in the order
        // of assignment determined above, map each string to a four-digit
        // number.
        int num = 0;
        for (int i = 0; i < 4; ++i)
        {
            num *= 10;
            std::string input, bitpattern;
            istr.ignore();
            istr >> input;
            for (auto j : input)
            {
                bitpattern.append(1, static_cast<char>(bits[j]) + '0');
            }
            std::sort(bitpattern.begin(), bitpattern.end());
            num += values.at(bitpattern);
        }
        count += num;
    }

    std::cout << count << '\n';
    return 0;
}
