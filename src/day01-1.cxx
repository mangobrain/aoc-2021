#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

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

    // First line doesn't count - previous line is "N/A". Fake this by
    // simply starting one below zero, and always treating the first
    // line of input as an increase.
    int prev = 0;
    int numIncreases = -1;

    for (std::string line; (!fstr.eof())
            && (fstr.peek() != std::ifstream::traits_type::eof())
            && std::getline(fstr, line); )
    {
        istr.clear();
        istr.str(line);

        int current;
        istr >> current;

        if (current > prev)
            ++numIncreases;

        prev = current;
    }

    std::cout << numIncreases << '\n';
    return 0;
}
