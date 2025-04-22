#include <sstream>

using namespace std;

string decode(const string &str)
{
    string decode;
    istringstream iss(str);
    char c;
    int i;

    while (iss.get(c))
    {
        if (c == '%')
        {
            if (iss >> hex >> i)
            {
                decode += static_cast<char>(i);
            }
        }
        else if (c == '+')
        {
            decode += ' ';
        }
        else
        {
            decode += c;
        }
    }
    return decode;
}