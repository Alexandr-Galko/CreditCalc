#pragma once

#include <chrono>
#include <string>
#include <sstream>

class Date
{
public:
    Date(int month_, int year_)
        :month(month_),
          year(year_){}
    int month;
    int year;
    void monthInc()
    {
        if (++month > 12)
        {
            month = 1;
            ++year;
        }
    }
    std::tm toTm()
    {
        auto result = std::tm();
        result.tm_mon = month - 1;
        result.tm_year = year - 1900;
        return result;
    }
};

bool operator<(const Date& left, const Date& right);

Date stringToDate(std::string s);
