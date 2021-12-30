#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <cmath>
#include <limits>
#include <list>
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

    int high = 0, low = std::numeric_limits<int>::max();
    std::list<int> positions;
    while ((!fstr.eof()) && (fstr.peek() != std::ifstream::traits_type::eof()))
    {
        int position;
        fstr >> position;
        fstr.ignore();

        if (position > high)
            high = position;
        if (position < low)
            low = position;

        positions.push_back(position);
    }

    std::vector<int> costs((high - low) + 1, 0);

    for (auto p : positions)
    {
        // Start from the crab's initial position, and work outwards; one loop
        // from the crab up to the furthest point, and one from the crab down
        // to the lowest point. The increase in movement cost goes up by one
        // each time round the loop.
        int increment = 1, cost = 0;
        for (auto i = p + 1; i <= high; ++i)
        {
            cost += increment;
            ++increment;
            costs[i - low] += cost;
        }
        increment = 1;
        cost = 0;
        for (auto i = p - 1; i >= low; --i)
        {
            cost += increment;
            ++increment;
            costs[i - low] += cost;
        }
    }

    int cheapest = std::numeric_limits<int>::max();
    for (auto c : costs)
    {
        if (c < cheapest)
            cheapest = c;
    }

    std::cout << cheapest << '\n';
    return 0;
}
