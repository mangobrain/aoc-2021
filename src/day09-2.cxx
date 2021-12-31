#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <algorithm>
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

    typedef std::pair<int, bool> Square;

    std::vector<std::vector<Square>> grid;
    for (std::string line; (!fstr.eof())
            && (fstr.peek() != std::ifstream::traits_type::eof())
            && std::getline(fstr, line); )
    {
        // Surround the grid with a 1 digit thick border of 10s, so that
        // anything at the edge of the "real" grid is automatically lower
        std::vector<Square> row(1, Square{10, true});
        for (auto i : line)
        {
            row.push_back(Square{static_cast<int>(i - '0'), false});
        }
        row.push_back(Square{10, true});
        grid.push_back(row);
    }
    grid.insert(grid.begin(),
            std::vector<Square>(grid.front().size(), Square{10, true}));
    grid.push_back(std::vector<Square>(grid.front().size(), Square{10, true}));

    std::vector<int> basins;
    for (std::size_t y = 1; y < grid.size() - 1; ++y)
    {
        for (std::size_t x = 1; x < grid[y].size() - 1; ++x)
        {
            auto i = grid[y][x].first;
            if ((i < grid[y - 1][x].first) && (i < grid[y + 1][x].first)
                    && (i < grid[y][x - 1].first)
                    && (i < grid[y][x + 1].first))
            {
                // We have identified a low point. See how big its associated
                // basin is.
                int size = 0;
                std::stack<std::pair<std::size_t, std::size_t>> basin;
                grid[y][x].second = true;
                basin.emplace(y, x);
                while (!basin.empty())
                {
                    ++size;
                    auto c = basin.top();
                    basin.pop();
                    auto height = grid[c.first][c.second].first;
                    auto & up = grid[c.first - 1][c.second];
                    auto & down = grid[c.first + 1][c.second];
                    auto & left = grid[c.first][c.second - 1];
                    auto & right = grid[c.first][c.second + 1];
                    if ((up.first < 9) && (!up.second) && (up.first > height))
                    {
                        up.second = true;
                        basin.emplace(c.first - 1, c.second);
                    }
                    if ((down.first < 9) && (!down.second)
                            && (down.first > height))
                    {
                        down.second = true;
                        basin.emplace(c.first + 1, c.second);
                    }
                    if ((left.first < 9) && (!left.second)
                            && (left.first > height))
                    {
                        left.second = true;
                        basin.emplace(c.first, c.second - 1);
                    }
                    if ((right.first < 9) && (!right.second)
                            && (right.first > height))
                    {
                        right.second = true;
                        basin.emplace(c.first, c.second + 1);
                    }
                }
                basins.push_back(size);
            }
        }
    }

    std::sort(basins.begin(), basins.end());
    auto b = basins.rbegin();
    int answer = *(b++);
    for (int i = 1; (i < 3) && (b != basins.rend()); ++i, ++b)
    {
        answer *= *b;
    }

    std::cout << answer << '\n';
    return 0;
}
