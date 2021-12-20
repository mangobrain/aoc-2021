#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <cstdint>
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

    std::uint64_t population = 0;
    while (true)
    {
        int i;
        istr >> i;

        // Process the fish one by one.
        // We can keep a ring buffer of 7 pairs of values: how many offspring
        // there are with timers that hit zero, and how many offspring finish
        // their incubation period, on each day.
        std::pair<std::uint64_t, std::uint64_t> week[7] = {
            {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}
        };
        week[i].first = 1;
        int a = 0, b = 2;
        for (int day = 0; day < 256; ++day)
        {
            // Every fish whose counter is zero spawns a fish who, in two
            // days' time, will enter the pool of fertile fish for that day.
            week[b].second += week[a].first;

            // Every fish spawned two days ago enters today's fertile pool.
            week[a].first += week[a].second;
            week[a].second = 0;

            // Rotate through the week
            if (++a == 7)
                a = 0;
            if (++b == 7)
                b = 0;
        }

        // Add this fish and all its progeny to the population counter
        for (auto i : week)
        {
            population += i.first;
            population += i.second;
        }

        if (istr.eof())
            break;
        istr.ignore();
    }

    std::cout << population << '\n';
    return 0;
}
