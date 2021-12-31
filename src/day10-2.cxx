#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <algorithm>
#include <cstdint>
#include <map>
#include <stack>
#include <vector>

std::map<char, int> const scores{
    {'(', 1},
    {'[', 2},
    {'{', 3},
    {'<', 4},
};

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

    std::vector<std::uint64_t> totals;
    for (std::string line; (!fstr.eof())
            && (fstr.peek() != std::ifstream::traits_type::eof())
            && std::getline(fstr, line); )
    {
        bool corrupt = false;
        std::stack<char> chunks;
        for (auto i : line)
        {
            char j = 0;
            switch (i)
            {
                case '(':
                case '[':
                case '{':
                case '<':
                    chunks.push(i);
                    break;

                // ( and ) are next to each other in character value
                // All the others have a single intervening character
                case ']':
                case '}':
                case '>':
                    ++j;
                    [[fallthrough]];
                case ')':
                    ++j;
                    if (chunks.top() == i - j)
                        chunks.pop();
                    else
                        corrupt = true;
            }
            if (corrupt)
                break;
        }

        if (corrupt)
            continue;
        // Line is not corrupt, but it might be incomplete
        std::uint64_t score = 0;
        while (!chunks.empty())
        {
            score *= 5;
            score += scores.at(chunks.top());
            chunks.pop();
        }
        totals.push_back(score);
    }

    std::sort(totals.begin(), totals.end());
    std::cout << totals[totals.size() / 2] << '\n';
    return 0;
}
