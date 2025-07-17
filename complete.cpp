#include <windows.h>
#undef max //取消windows.h中的max宏定义
#include<iostream>
#include<string>
#include<cctype>
#include<limits>
class Calculate
{
    public:
        double add(double a, double b) {return a + b;}
        double sub(double a, double b) {return a - b;}
        double mul(double a, double b) {return a * b;}
        struct DivResult
        {
            double realValue;
            bool error;
            std::string errorMsg;
            DivResult(double v, bool e, std::string msg) : realValue(v), error(e), errorMsg(msg) {}
        };
        DivResult div(double a, double b) {
            if (b == 0) return DivResult(0, true, "除数再为tou");
            else return DivResult(a / b, false, "");
        }
};



int main()
{
    SetConsoleOutputCP(65001);
    Calculate cal;
    char op, flag = '\0';
    double a, b;
    while(flag != 'q')
    {
        // 运算符输入校验
        while (true) {
            std::cout << "请输入运算类型 (+ - * /)：" << std::endl;
            std::cin >> op;
            if (op == '+' || op == '-' || op == '*' || op == '/') break;
            else std::cout << "好好输入运算符，再乱输就把你头打爆(#`д´)ﾉ " << std::endl;
        }
        // 数字输入
        while(true)
        {
            std::cout << "请输入运算数字（注意顺序）：" << std::endl;
            std::cin >> a >> b;
            if(std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "好好输入数字，再乱输就把你头打爆(#`д´)ﾉ " << std::endl;
            } else {
                break;
            }
        }
        switch(op)
        {
            case '+' : std::cout << cal.add(a, b) << std::endl; break;
            case '-' : std::cout << cal.sub(a, b) << std::endl; break;
            case '*' : std::cout << cal.mul(a, b) << std::endl; break;
            case '/' : {
                auto result = cal.div(a, b);
                if(result.error) std::cout << result.errorMsg << std::endl;
                else std::cout << result.realValue << std::endl;
                break;
            }
        }
        std::cout << "是否继续，按'q'结束" << std::endl;
        std::cin >> flag;
    }
}

