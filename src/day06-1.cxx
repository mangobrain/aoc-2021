#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <stack>
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

    std::string line;
    std::getline(fstr, line);
    istr.clear();
    istr.str(line);

    std::stack<std::pair<int, int>> fishes;

    while (true)
    {
        int i;
        istr >> i;
        fishes.emplace(i, 80);
        if (istr.eof())
            break;
        istr.ignore();
    }

    // Process fish one by one - spawn all the new fishes
    int population = fishes.size();
    while (!fishes.empty())
    {
        auto fish = fishes.top();
        fishes.pop();
        while (--fish.second >= 0)
        {
            if (fish.first == 0)
            {
                fish.first = 6;
                fishes.emplace(8, fish.second);
                ++population;
            }
            else
                --fish.first;
        }
    }

    std::cout << population << '\n';
    return 0;
}
