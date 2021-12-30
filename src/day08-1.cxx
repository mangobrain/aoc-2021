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

    int count = 0;
    for (std::string line; (!fstr.eof())
            && (fstr.peek() != std::ifstream::traits_type::eof())
            && std::getline(fstr, line); )
    {
        istr.clear();
        istr.str(line);

        // Grab each of the 10 unique signal patterns.
        // We don't actually need these for day 1, but it gets us up to the
        // divider.
        std::string combinations[10];
        for (auto & c : combinations)
        {
            istr >> c;
            istr.ignore();
        }

        // Ignore pipe character
        istr.ignore();

        // Grab the four signals corresponding to the displayed digits,
        // ignoring leading space.
        std::string digits[4];
        for (auto & d : digits)
        {
            istr.ignore();
            istr >> d;
        }

        // 1: 2 segments lit
        // 4: 4 segments lit
        // 7: 3 segments lit
        // 8: 7 segments lit
        for (auto & d : digits)
        {
            switch (d.length())
            {
                case 2:
                case 4:
                case 3:
                case 7:
                    ++count;
            }
        }
    }

    std::cout << count << '\n';
    return 0;
}
