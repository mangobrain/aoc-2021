#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <set>
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

    std::istringstream istr;
    istr.exceptions(std::ios_base::badbit | std::ios_base::failbit);

    // Point coordinates
    std::vector<std::pair<int, int>> points;
    while (true)
    {
        std::string line;
        std::getline(fstr, line);
        istr.clear();
        istr.str(line);

        // Blank line at end of coordinates
        if (line.empty())
            break;

        int a, b;
        istr >> a;
        istr.ignore();
        istr >> b;

        points.emplace_back(a, b);
    }

    // Fold instructions
    for (std::string line; (!fstr.eof())
            && (fstr.peek() != std::ifstream::traits_type::eof())
            && std::getline(fstr, line); )
    {
        istr.clear();
        istr.str(line);

        // Example:
        // fold along x=5
        if (line.length() < 14)
            return 1;

        char axis = line[11];
        istr.ignore(13);

        int position;
        istr >> position;

        // "fold along x=5" actually means fold along a vertical line that goes
        // through x = 5 - so it's the axis & position of a point along the
        // fold line, *not* the axis along which to fold.
        // Perform the fold.
        if (axis == 'x')
        {
            // Fold along a vertical line.
            // Points to the right of the line are mirrored along it.
            for (auto & p : points)
            {
                if (p.first > position)
                    p.first = position - (p.first - position);
            }
        }
        else
        {
            // Fold along a horizontal line.
            // Points below the line are mirrored along it.
            for (auto & p : points)
            {
                if (p.second > position)
                    p.second = position - (p.second - position);
            }
        }
    }

    // Output the resulting grid
    std::set<std::pair<int, int>> uniq;
    int w = 0, h = 0;
    for (auto p : points)
    {
        if (uniq.insert(p).second)
        {
            if (p.first > w)
                w = p.first;
            if (p.second > h)
                h = p.second;
        }
    }
    for (int y = 0; y <= h; ++y)
    {
        for (int x = 0; x <= w; ++x)
        {
            if (uniq.count({x, y}))
                std::cout << '#';
            else
                std::cout << '.';
        }
        std::cout << '\n';
    }

    return 0;
}
