#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <map>
#include <stack>

std::map<char, int> const scores{
    {')', 3},
    {']', 57},
    {'}', 1197},
    {'>', 25137},
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

    int score = 0;
    for (std::string line; (!fstr.eof())
            && (fstr.peek() != std::ifstream::traits_type::eof())
            && std::getline(fstr, line); )
    {
        std::stack<char> chunks;
        for (auto i : line)
        {
            char j = 0;
            bool corrupt = false;
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
                    {
                        corrupt = true;
                        score += scores.at(i);
                    }
            }
            if (corrupt)
                break;
        }
    }

    std::cout << score << '\n';
    return 0;
}
