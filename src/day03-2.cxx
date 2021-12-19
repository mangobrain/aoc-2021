#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <map>
#include <utility>
#include <vector>

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

    // Keep track of all the numbers; we're going to need them later.
    std::vector<int> numbers;
    int digits;

    for (std::string line; (!fstr.eof())
            && (fstr.peek() != std::ifstream::traits_type::eof())
            && std::getline(fstr, line); )
    {
        digits = line.length();
        // Convert all the numbers from binary to decimal
        int number = 0;
        for (auto i : line)
        {
            number <<= 1;
            if (i == '1')
                ++number;
        }
        numbers.push_back(number);
    }

    // Start with the most significant digit
    int index = digits - 1;
    int mask = 0;
    auto oxygen{numbers};
    while (oxygen.size() > 1)
    {
        std::vector<int> newOxygen;
        // Out of the numbers that remain, for the current digit,
        // do we have more ones, or more zeroes?
        int ones = 0, zeroes = 0;
        for (auto i : oxygen)
        {
            if (i & (1 << index))
                ++ones;
            else
                ++zeroes;
        }
        // Keep numbers with the most common digit at this position
        // In a tie-breaker situation, keep numbers with a 1 at this index
        if (ones >= zeroes)
            mask += 1 << index;
        // Keep only what matches our filter so far
        for (auto i : oxygen)
        {
            if (((i >> index) << index) == mask)
                newOxygen.push_back(i);
        }
        oxygen.swap(newOxygen);
        --index;
    }

    index = digits - 1;
    mask = 0;
    auto carbon{numbers};
    while (carbon.size() > 1)
    {
        std::vector<int> newCarbon;
        int ones = 0, zeroes = 0;
        for (auto i : carbon)
        {
            if (i & (1 << index))
                ++ones;
            else
                ++zeroes;
        }
        // Keep numbers with the least common digit at this position
        // In a tie-breaker situation, keep numbers with a 0 at this index
        if (ones < zeroes)
        {
            mask += 1 << index;
        }
        for (auto i : carbon)
        {
            if (((i >> index) << index) == mask)
                newCarbon.push_back(i);
        }
        carbon.swap(newCarbon);
        --index;
    }

    std::cout << "Oxygen: " << oxygen[0] << ", carbon: " << carbon[0] << '\n'
        << (oxygen[0] * carbon[0]) << '\n';

    return 0;
}
