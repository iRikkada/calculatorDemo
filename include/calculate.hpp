#ifndef CALCULATE_H
#define CALCULATE_H
#include "tool.hpp"
class Calculate
{
    public:
        struct Result {
            double realValue;
            std::string errorMsg;
            bool error;
            Result(double value = 0.0, const std::string& msg = "", bool err = false)
                : realValue(value), errorMsg(msg), error(err) {};
        };
    Result eval(std::string& expression);
    private:
    std::unordered_map<std::string, std::function<double(double)>> functions = {
        {"sin", [](double x) { return std::sin(x); }},
        {"cos", [](double x) { return std::cos(x); }},
        {"tan", [](double x) { return std::tan(x); }},
        {"ln", [](double x) { return std::log(x); }},
        {"lg", [](double x) { return std::log10(x); }},
        {"sqrt", [](double x) { return std::sqrt(x); }}
    };
};


#endif 