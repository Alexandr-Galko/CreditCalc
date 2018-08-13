#include "date.h"

bool operator<(const Date &left, const Date &right)
{
    return ((left.year << 8) | left.month) < ((right.year << 8) | right.month);
}

Date stringToDate(std::string s)
{
    std::stringstream ss;
    ss << s;
    std::string smonth;
    std::string syear;
    int month;
    int year;

    ss >> smonth >> syear;
    month = stoi(smonth);
    year = stoi(syear);
    return Date(month, year);
}
