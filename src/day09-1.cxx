#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <cstdint>
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

    std::vector<std::vector<int>> grid;
    for (std::string line; (!fstr.eof())
            && (fstr.peek() != std::ifstream::traits_type::eof())
            && std::getline(fstr, line); )
    {
        // Surround the grid with a 1 digit thick border of 10s, so that
        // anything at the edge of the "real" grid is automatically lower
        std::vector<int> row(1, 10);
        for (auto i : line)
        {
            row.push_back(static_cast<int>(i - '0'));
        }
        row.push_back(10);
        grid.push_back(row);
    }
    grid.insert(grid.begin(), std::vector<int>(grid.front().size(), 10));
    grid.push_back(std::vector<int>(grid.front().size(), 10));

    int risk = 0;
    for (std::size_t y = 1; y < grid.size() - 1; ++y)
    {
        for (std::size_t x = 1; x < grid[y].size() - 1; ++x)
        {
            auto i = grid[y][x];
            if ((i < grid[y - 1][x]) && (i < grid[y + 1][x])
                    && (i < grid[y][x - 1]) && (i < grid[y][x + 1]))
            {
                risk += 1 + i;
            }
        }
    }

    std::cout << risk << '\n';
    return 0;
}
