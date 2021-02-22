#include <sstream>
#include <string>

#include "format.h"

using std::string;

string Format::ElapsedTime(long seconds)
{
    // divide seconds by 60 to get minutes
    long m = seconds / 60;

    // seconds modulus of 60 to get remaining seconds
    long s = seconds % 60;

    // divide hours by 24 to get remaining days
    long h = m / 60;

    // hours modulus of 24 to get remaining minutes
    m = m % 60;
    
    std::stringstream ss; 

    if (h < 10) ss << "0";
    ss << std::to_string(h)  << ":";
    
    if (m < 10) ss << "0";
    ss << std::to_string(m) << ":";

    if (s < 10) ss << "0";
    ss << std::to_string(s);
    
    return ss.str();
}