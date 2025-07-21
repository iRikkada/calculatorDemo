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
};


#endif 