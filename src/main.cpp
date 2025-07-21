#include <windows.h>
#undef max //取消windows.h中的max宏定义
#include"../include/calculate.hpp"

int main()
{
    SetConsoleOutputCP(65001);
    Calculate cal;
    std::string expression = "";
    while(true) {
        std::cout << "请输入表达式，输入'quit'退出程序" << std::endl;
        std::getline(std::cin, expression);
        if(expression == "quit") {
            break;
        }
        if(expression.empty()) {
            std::cout << "输入不能为空，请重新输入" << std::endl;
            continue;
        }
        Calculate::Result result = cal.eval(expression);
        if(result.error) {
            std::cout << "计算错误: " << result.errorMsg << std::endl;
        }
        else {
            std::cout << "计算结果: " << result.realValue << std::endl;
        }
    }
    return 0;
}

