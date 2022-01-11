#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <cstdint>
#include <limits>
#include <queue>
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

    // Grab starting state
    std::string line;
    std::getline(fstr, line);

    // Insert initial pairs present at step 0
    std::vector<std::uint64_t> pairs(65536, 0);
    auto last = line[0];
    for (std::size_t i = 1; i < line.length(); ++i)
    {
        int pair = (static_cast<int>(last) << 8)
            + line[i];
        pairs[pair] += 1;
        last = line[i];
    }

    // When counting final pairs later, to avoid double counting, we will only
    // consider the right-hand character. Prime the count with a single
    // occurrence of the left-most character, so that it doesn't get lost.
    std::vector<std::uint64_t> count(256, 0);
    count[line[0]] = 1;

    // Ignore blank line
    std::getline(fstr, line);

    // Grab rules
    std::vector<int> rules(65536, 0);
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
        int match = (static_cast<int>(line[0]) << 8)
            + line[1];
        rules[match] = line[6];
    }

    // Apply rules for 40 steps, and count final occurrences of each character
    for (int j = 0; j < 40; ++j)
    {
        std::vector<std::uint64_t> newpairs(65536, 0);
        for (int i = 0; i < 65536; ++i)
        {
            if (pairs[i] > 0)
            {
                if (rules[i] > 0)
                {
                    newpairs[(i & 0xff00) + rules[i]] += pairs[i];
                    newpairs[(i & 0xff) + (rules[i] << 8)] += pairs[i];
                }
                else
                    count[i & 0xff] += pairs[i];
            }
        }
        pairs.swap(newpairs);
    }

    // Sum final occurrences
    for (int i = 0; i < 65536; ++i)
    {
        count[i & 0xff] += pairs[i];
    }

    std::uint64_t low = std::numeric_limits<std::uint64_t>::max(), high = 0;
    char i = 0;
    for (auto c : count)
    {
        if (c > high)
            high = c;
        if ((c > 0) && (c < low))
            low = c;
        ++i;
    }
    std::cout << (high - low) << '\n';
    return 0;
}
