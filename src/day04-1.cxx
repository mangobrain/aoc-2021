#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <list>
#include <map>
#include <memory>
#include <tuple>
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

    std::istringstream istr;
    istr.exceptions(std::ios_base::badbit | std::ios_base::failbit);

    // Grab first line of input: the numbers drawn
    std::vector<int> drawn;
    {
        std::string line;
        std::getline(fstr, line);
        istr.str(line);
        while (true)
        {
            int i;
            istr >> i;
            drawn.push_back(i);
            if (istr.eof())
                break;
            // Not at end of line - skip comma
            istr.ignore();
        }
    }

    // Grab the remainder of the input: the bingo cards
    typedef std::vector<std::vector<int>> Board;
    std::vector<std::unique_ptr<Board>> boards;
    // Also keep a map of <board, x, y> coordinates for each number
    typedef std::tuple<int, int, int> Coords;
    std::map<int, std::list<Coords>> positions;
    {
        int b = -1, y = 0;
        std::unique_ptr<Board> current;
        for (std::string line; (!fstr.eof())
                && (fstr.peek() != std::ifstream::traits_type::eof())
                && std::getline(fstr, line); )
        {
            if (line.empty())
            {
                // Blank line - set up for new board
                if (current)
                    boards.push_back(std::move(current));
                current.reset(new Board);
                y = 0;
                ++b;
            }
            else
            {
                // Next row of existing board
                istr.clear();
                istr.str(line);
                std::vector<int> row;
                int x = 0;
                while (!istr.eof())
                {
                    int i;
                    istr >> i;
                    row.push_back(i);
                    positions[i].emplace_back(b, x, y);
                    ++x;
                }
                current->push_back(row);
                ++y;
            }
        }
        // Make sure we don't forget to store the final board
        boards.push_back(std::move(current));
    }

    // For each number drawn, add 1 to the total for the row & column of each
    // board where it is present. If this brings the total for the row/column
    // to the width/height, we have a winner.
    int width = (*boards[0])[0].size();
    int height = boards[0]->size();
    std::vector<std::vector<int>> rows(boards.size(), std::vector(height, 0));
    std::vector<std::vector<int>> cols(boards.size(), std::vector(width, 0));
    int draws = 0, winningBoard = 0;
    for (auto i : drawn)
    {
        bool won = false;
        for (auto j : positions[i])
        {
            int b, x, y;
            std::tie(b, x, y) = j;
            // Add to the total
            auto & row = rows[b][y];
            auto & col = cols[b][x];
            // Mark off the number
            (*boards[b])[y][x] = -1;
            // Check for a win
            if ((++row == width) || (++col == height))
            {
                winningBoard = b;
                won = true;
                break;
            }
        }
        if (won)
            break;
        ++draws;
    }

    // Get the sum of all unmarked numbers on the winning board
    int sum = 0;
    for (auto & i : *boards[winningBoard])
    {
        for (auto j : i)
        {
            if (j > -1)
                sum += j;
        }
    }

    std::cout << sum << '*' << drawn[draws] << ": " << (sum * drawn[draws])
        << '\n';
    return 0;
}
