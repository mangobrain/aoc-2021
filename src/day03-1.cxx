#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <map>
#include <utility>

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

    // Keep track of the number of times each bit occurs in each position.
    // Agnostic to the length of binary numbers in the input, as long as
    // they're all the same length as each other.
    std::map<int, std::pair<int, int>> digits;

    for (std::string line; (!fstr.eof())
            && (fstr.peek() != std::ifstream::traits_type::eof())
            && std::getline(fstr, line); )
    {
        int index = 0;
        for (auto i : line)
        {
            if (i == '0')
                digits[index].first += 1;
            else
                digits[index].second += 1;
            ++index;
        }
    }

    int gamma = 0;
    int epsilon = 0;

    // Our "digit 0" is actually the most significant digit; work backwards.
    int index = digits.size() - 1;
    for (auto elem : digits)
    {
        auto d = elem.second;
        if (d.second > d.first)
            gamma += 1 << index;
        else
            epsilon += 1 << index;
        --index;
    }

    std::cout << "Gamma: " << gamma << ", epsilon: " << epsilon << '\n';
    std::cout << "Gamma x epsilon: " << (gamma * epsilon) << '\n';
    return 0;
}
