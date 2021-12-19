#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
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

    std::istringstream istr;
    istr.exceptions(std::ios_base::badbit | std::ios_base::failbit);

    int prev = 0;
    int numIncreases = -1;
    std::pair<int, int> windows[3] = {
        {0, -2},
        {0, -1},
        {0, 0}
    };

    for (std::string line; (!fstr.eof())
            && (fstr.peek() != std::ifstream::traits_type::eof())
            && std::getline(fstr, line); )
    {
        istr.clear();
        istr.str(line);

        int current;
        istr >> current;

        for (auto & p : windows) {
            if (p.second >= 0)
                p.first += current;
            if (++p.second == 3)
            {
                if (p.first > prev)
                    ++numIncreases;
                prev = p.first;
                p.first = p.second = 0;
            }
        }
    }

    std::cout << numIncreases << '\n';
    return 0;
}
