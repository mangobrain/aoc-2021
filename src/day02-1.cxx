#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

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

    int horz = 0;
    int depth = 0;

    for (std::string line; (!fstr.eof())
            && (fstr.peek() != std::ifstream::traits_type::eof())
            && std::getline(fstr, line); )
    {
        istr.clear();
        istr.str(line);

        std::string direction;
        int amount = 0;

        istr >> direction >> amount;

        switch (direction[0]) {
            case 'f':
                horz += amount;
                break;
            case 'u':
                depth -= amount;
                break;
            default:
                depth += amount;
        }
    }

    std::cout << "Horizontal position: " << horz << ", depth: " << depth
        << '\n';
    std::cout << (horz * depth) << '\n';
    return 0;
}
