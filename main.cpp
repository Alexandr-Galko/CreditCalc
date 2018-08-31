#include <iostream>
#include <ctime>
#include <iomanip>
#include <map>
#include <tuple>
#include "date.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/program_options.hpp>

namespace pt = boost::property_tree;
namespace po = boost::program_options;
using namespace std;

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        cerr<< "enter json file";
        return 1;
    }
    string filename = argv[1];
    cout << "рассчет кредита" << endl;
    pt::ptree root;
    pt::read_json(filename, root);
    map<Date, double> precents;
    for (auto& item: root.get_child("precents"))
    {
        auto& item0 = item.second.front();
        auto d = stringToDate(item0.first);
        precents[d] = item0.second.get_value<double>();
    }

    map<Date, double> pays;
    for (auto& item: root.get_child("pays"))
    {
        auto& item0 = item.second.front();
        auto d = stringToDate(item0.first);
        pays[d] = item0.second.get_value<int>();
    }

    auto currentDatestr = root.get<string>("currentDate", "");
    auto currentDate = stringToDate(currentDatestr);

    int planedPay = root.get<int>("planedPay", 0);
    double balance = root.get<int>("debt", 0);
    int durationYears = root.get<int>("duration", 0);

    for (int i = 0 ; i < 20 && planedPay != 0; ++i)
    {
        pays[currentDate] = planedPay;
        currentDate.monthInc();
    }

    cout << "current " << currentDatestr << endl;
    Date currentdate = begin(precents)->first;
    double percent = begin(precents)->second;
    int duration = durationYears * 12;
    double debt = balance / duration;
    double pay;
    int overpay = 0;
    while (balance > 0)
    {
        currentdate.monthInc();
        auto currentPercent = precents.find(currentdate);
        percent = currentPercent != end(precents) ? currentPercent->second : percent;
        double percentages = balance * (percent/100/12);

        overpay += percentages;

        pay = debt + percentages;

        auto currentPay = pays.find(currentdate);
        pay = currentPay != end(pays) ? currentPay->second : pay;

        pay = pay < (balance + percentages)? pay : (balance + percentages);
        balance = balance - (pay - percentages);
        auto t = currentdate.toTm();
        cout << "период: " << put_time(&t, "%B %t %Y");
        cout << " выплата: " << pay;
        cout << " долг: " << (pay - percentages);
        cout << " проценты: " << percentages;
        cout << " остаток: " << balance;
        cout << " процент: " << percent;
        cout << " переплата: " << overpay;
        cout << endl;
    }
    return 0;
}
