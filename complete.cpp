#include <windows.h>
#undef max //取消windows.h中的max宏定义
#include<iostream>
#include<string>
#include<cctype>
#include<limits>
#include<stack>
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
        Result eval(const std::string& expression);
};

// 判断运算符的优先级 若op1的优先级小于等于op2则返回true
int precedence(char op) {
    if(op == '+' || op == '-') return 1;
    if(op == '*' || op == '/') return 2;
    return 0;
}

bool isHigherPrecedence(char op1, char op2) {
    return precedence(op1) >= precedence(op2);
}

//计算表达式函数
Calculate::Result Calculate::eval(const std::string& expression)
{
    std::stack<double> values; //存储数字
    std::stack<char> ops; //存储运算符
    for (size_t i = 0; i < expression.length(); ++i) {
        // 跳过空格
        if (isspace(expression[i])) continue;
        // 如果是数字，提取完整的数字
        if (isdigit(expression[i]) || expression[i] == '.') {
            std::string numStr;
            while (i < expression.length() && (isdigit(expression[i]) || expression[i] == '.')) {
                numStr += expression[i++];
            }
            double num = std::stod(numStr);
            values.push(num);
            --i; // 因为for循环还会自增一次
        }
        // 如果是四则运算符
        // 如果是四则运算符
        else if (expression[i] == '+' || expression[i] == '-' || expression[i] == '*' || expression[i] == '/') {
            // 处理栈顶运算符的优先级
            while(!ops.empty() && ops.top() != '(' && isHigherPrecedence(ops.top(), expression[i])) {
                char op = ops.top();
                ops.pop();
                if(values.size() < 2) {
                    return Result(0.0, "表达式错误：运算数字数量不足", true);
                }
                double b = values.top(); values.pop();
                double a = values.top(); values.pop();
                double result = 0.0;
                switch(op) {
                    case '+': result = a + b; break;
                    case '-': result = a - b; break;
                    case '*': result = a * b; break;
                    case '/':
                        if(b == 0) {
                            return Result(0.0, "谁教你的让0当除数(╯▔皿▔)╯", true);
                        }
                        result = a / b;
                        break;
                    default:
                        return Result(0.0, "程序员，酒吧，炒面.jpg", true);
                }
                values.push(result);
            }
            ops.push(expression[i]);
        }
        // 如果是括号
        else if (expression[i] == '(') {
            ops.push(expression[i]);
        }
        // 如果是右括号
        else if (expression[i] == ')') {
            if(ops.empty()) {
                return Result(0.0, "Bro忘记输左括号了吧", true);
            }
            while(ops.top() !='(')
            {
                char op = ops.top();
                ops.pop();
                if(values.size() < 2) {
                    return Result(0.0, "表达式错误：运算数字数量不足", true);
                }
                double b = values.top(); values.pop();
                double a = values.top(); values.pop();
                double result = 0.0;
                switch(op) {
                    case '+': result = a + b; break;
                    case '-': result = a - b; break;
                    case '*': result = a * b; break;
                    case '/':
                        if(b == 0) {
                            return Result(0.0, "谁教你的让0当除数(╯▔皿▔)╯", true);
                        }
                        result = a / b;
                        break;
                    default:
                        return Result(0.0, "程序员，酒吧，炒面.jpg", true);
                }
                values.push(result);
            }
            ops.pop(); // 弹出左括号
        }
        else {
            return Result(0.0, "程序员，酒吧，炒面.jpg", true);
        }
    }
    // 剩余运算符
    while(!ops.empty()) {
        char op = ops.top();
        ops.pop();
        if(values.size() < 2) {
            return Result(0.0, "表达式错误：运算数字数量不足", true);
        }
        double b = values.top(); values.pop();
        double a = values.top(); values.pop();
        double result = 0.0;
        switch(op) {
            case '+': result = a + b; break;
            case '-': result = a - b; break;
            case '*': result = a * b; break;
            case '/':
                if(b == 0) {
                    return Result(0.0, "谁教你的让0当除数(╯▔皿▔)╯", true);
                }
                result = a / b;
                break;
            default:
                return Result(0.0, "程序员，酒吧，炒面.jpg", true);
        }
        values.push(result);
    }
    if (values.size() == 1)
        {return Result(values.top());}
    else
        return Result(0.0, "表达式错误：结果异常", true);
}

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

