#include "decode.h"
#include <sstream>

using namespace std;

// HTML FORM 인코딩 된 것을 디코딩
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