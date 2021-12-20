#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <algorithm>
#include <cmath>
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

    // Scan through once to get dimensions
    int width = 0, height = 0;
    for (std::string line; (!fstr.eof())
            && (fstr.peek() != std::ifstream::traits_type::eof())
            && std::getline(fstr, line); )
    {
        istr.clear();
        istr.str(line);

        // X1,Y1 -> X2,Y2
        int x1, y1, x2, y2;
        istr >> x1;
        istr.ignore();
        istr >> y1;
        istr.ignore(4);
        istr >> x2;
        istr.ignore();
        istr >> y2;

        if (x1 > width)
            width = x1;
        if (x2 > width)
            width = x2;
        if (y1 > height)
            height = y1;
        if (y2 > height)
            height = y2;
    }

    std::vector<std::vector<int>> grid(width+1, std::vector<int>(height+1, 0));

    // Scan through again and draw the vertical, horizontal, and diagonal lines
    // Keep track of the number of points where there is overlap
    fstr.seekg(0);
    fstr.clear();
    int overlaps = 0;
    for (std::string line; (!fstr.eof())
            && (fstr.peek() != std::ifstream::traits_type::eof())
            && std::getline(fstr, line); )
    {
        istr.clear();
        istr.str(line);

        // X1,Y1 -> X2,Y2
        int x1, y1, x2, y2;
        istr >> x1;
        istr.ignore();
        istr >> y1;
        istr.ignore(4);
        istr >> x2;
        istr.ignore();
        istr >> y2;

        // Check it is horizontal, vertical, or exactly 45 degrees diagonal
        if (x1 == x2)
        {
            // Vertical
            if (y1 > y2)
                std::swap(y1, y2);
            for (int y = y1; y <= y2; ++y)
            {
                // Just count the overlap once, when it reaches two
                // overlapping lines - not again if more lines overlap
                // the same point
                if (++(grid[x1][y]) == 2)
                    ++overlaps;
            }
        }
        else if (y1 == y2)
        {
            // Horizontal
            if (x1 > x2)
                std::swap(x1, x2);
            for (int x = x1; x <= x2; ++x)
            {
                if (++(grid[x][y1]) == 2)
                    ++overlaps;
            }
        }
        else if (std::abs(x1 - x2) == std::abs(y1 - y2))
        {
            // 45 degree diagonal
            // Go from left to right, up or down accordingly
            if (x1 > x2)
            {
                std::swap(x1, x2);
                std::swap(y1, y2);
            }
            int ydir = 1;
            if (y2 < y1)
                ydir = -1;
            for (int x = x1, y = y1; x <= x2; ++x, y += ydir)
            {
                if (++(grid[x][y]) == 2)
                    ++overlaps;
            }
        }
    }

    std::cout << overlaps << '\n';
    return 0;
}
