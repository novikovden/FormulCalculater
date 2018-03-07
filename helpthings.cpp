#include "helpthings.h"

std::string extractName(std::string::const_iterator b, std::string::const_iterator e, int *p) {
    std::string result;
    if(isalpha(*b) || *b == '_')
        result.push_back(*b);
    else {
        if(p)
            *p = 0;
        return "";
    }
    ++b;
    int pos = 1;
    while(b < e && (isdigit(*b) || isalpha(*b) || *b == '_')) {
        result.push_back(*(b++));
        ++pos;
    }
    if(p)
        *p = pos;
    return result;
}
