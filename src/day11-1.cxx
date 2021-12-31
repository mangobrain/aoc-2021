#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <cstdint>
#include <stack>
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

    typedef std::pair<char, bool> Square;

    std::vector<std::vector<Square>> grid;
    for (std::string line; (!fstr.eof())
            && (fstr.peek() != std::ifstream::traits_type::eof())
            && std::getline(fstr, line); )
    {
        std::vector<Square> row(1, {'0', true});
        for (auto i : line)
        {
            row.emplace_back(i, false);
        }
        row.emplace_back('0', true);
        grid.push_back(row);
    }
    grid.insert(grid.begin(),
            std::vector<Square>(grid[0].size(), {'0', true}));
    grid.push_back(std::vector<Square>(grid[0].size(), {'0', true}));

    std::uint64_t total = 0;
    for (int i = 0; i < 100; ++i)
    {
        // Reset all except the border to "not flashed"
        // Increase energy by 1; keep note of any flashes this causes
        std::stack<std::pair<std::size_t, std::size_t>> flashes;
        for (std::size_t y = 1; y < grid.size() - 1; ++y)
        {
            for (std::size_t x = 1; x < grid[y].size() - 1; ++x)
            {
                grid[y][x].second = false;
                if (++(grid[y][x].first) > '9')
                {
                    grid[y][x] = {'0', true};
                    flashes.emplace(y, x);
                    ++total;
                }
            }
        }
        // Keep increasing energy of adjacent octopuses, honouring any new
        // flashes, until we have nothing left to process
        while (!flashes.empty())
        {
            auto c = flashes.top();
            flashes.pop();
            for (auto y = c.first - 1; y <= c.first + 1; ++y)
            {
                for (auto x = c.second - 1; x <= c.second + 1; ++x)
                {
                    if (c == std::pair<std::size_t, std::size_t>{y, x})
                        continue;
                    if (grid[y][x].second)
                        continue;
                    if (++(grid[y][x].first) > '9')
                    {
                        grid[y][x] = {'0', true};
                        flashes.emplace(y, x);
                        ++total;
                    }
                }
            }
        }
    }

    std::cout << total << '\n';
    return 0;
}
