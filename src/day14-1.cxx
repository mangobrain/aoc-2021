#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <cstdint>
#include <limits>
#include <map>
#include <queue>
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

    // Grab starting state
    std::string line;
    std::getline(fstr, line);

    // Insert initial pairs present at step 0
    std::queue<std::pair<std::uint16_t, int>> pairs;
    std::uint8_t last = line[0];
    for (std::size_t i = 1; i < line.length(); ++i)
    {
        std::uint16_t pair = (static_cast<std::uint16_t>(last) << 8)
            + line[i];
        pairs.emplace(pair, 0);
        last = line[i];
    }

    // Ignore blank line
    std::getline(fstr, line);

    // Grab rules
    std::map<std::uint16_t, std::uint16_t> rules;
    for (; (!fstr.eof())
            && (fstr.peek() != std::ifstream::traits_type::eof())
            && std::getline(fstr, line); )
    {
        // Example: AA -> B
        if (line.length() != 7)
        {
            std::cout << line << '\n';
            return 1;
        }
        std::uint16_t match = (static_cast<std::uint16_t>(line[0]) << 8)
            + line[1];
        rules[match] = line[6];
    }

    // Apply rules for 10 steps, and count final occurrences of each character
    // For each pair we will only count the right-hand character, to avoid
    // double counting, so we must prime the count with the left-most character
    std::map<char, int> count{{pairs.front().first >> 8, 1}};
    while (!pairs.empty())
    {
        auto p = pairs.front();
        auto r = rules.find(p.first);
        if ((r == rules.end()) || (p.second == 10))
            count[p.first & 0xff] += 1;
        else
        {
            pairs.emplace((p.first & 0xff00) + r->second, p.second + 1);
            pairs.emplace((p.first & 0xff) + (r->second << 8), p.second + 1);
        }
        pairs.pop();
    }

    std::pair<char, int> low{' ', std::numeric_limits<int>::max()},
        high{' ', 0};
    for (auto c : count)
    {
        if (c.second > high.second)
            high = c;
        if (c.second < low.second)
            low = c;
    }
    std::cout << (high.second - low.second) << '\n';
    return 0;
}
