#include <windows.h>
#undef max //取消windows.h中的max宏定义
#include<iostream>
#include<algorithm>
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
        Result eval(std::string& expression);
};

//检验括号的匹配性
bool checkBrackets(const std::string& expr) {
    std::stack<char> s;
    for (char c : expr) {
        if (c == '(') s.push(c);
        else if (c == ')') {
            if (s.empty() || s.top() != '(') return false;
            s.pop();
        }
    }
    return s.empty();
}

//检验操作符和操作数的位置
bool checkOperators(const std::string& expr){
    if(expr[0] == '+' || expr[0] == '*' || expr[0] == '/') return false; //开头不能出现+*/
    if(expr[expr.size() - 1] == '+' || expr[expr.size() - 1] == '*' || expr[expr.size() - 1] == '/' || expr[expr.size() - 1] == '-') return false; //尾部不能出现+-*/
    for(size_t i = 1 ; i < expr.size(); i++){
        if(expr[i] == '+' || expr[i] == '-' || expr[i] == '*' || expr[i] == '/' ){
            char prev = expr[i - 1];
            char next = expr[i + 1];
            if((isdigit(prev) || prev == '(') && (isdigit(next) || prev == '(')){
                return true;
            }
        }
    }
    return false; //如果没有找到合法的操作符位置，则返回false
}

//检验字符的合法性
bool checkVariables(const std::string& expr) {
    for (char c : expr) {
        if (!isalnum(c) && c != '+' && c != '-' && c != '*' && c != '/' && c != '^' && c != '(' && c != ')') {
            return false; // 非法字符
        }
    }
    return true;
}

//综合检验
bool isValidInfix(const std::string& expr) {
    return checkBrackets(expr) && checkOperators(expr) && checkVariables(expr);
}

//移除所有空白字符
std::string removeSpaces(std::string& str) {
    str.erase(std::remove_if(str.begin(), str.end(), [](unsigned char c) -> bool {return std::isspace(c);}), str.end());
    return str;
}

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
Calculate::Result Calculate::eval(std::string& expression)
{
    removeSpaces(expression);//先移除字符串中的空白字符
    std::stack<double> values; //存储数字
    std::stack<char> ops; //存储运算符
    for (size_t i = 0; i < expression.length(); ++i) {
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
        else if (expression[i] == '+' || expression[i] == '-' || expression[i] == '*' || expression[i] == '/') {
            // 检查是否是负数
            if(expression[i] == '-' && (i == 0 || expression[i-1] == '(' )) {
                // 这是负号，和后面的数字一起解析
                std::string numStr;
                numStr += expression[i++]; //此时直接将-压入字符串中了，所以后面不需要压入-num了
                // while (i < expression.length() && (isdigit(expression[i]) || expression[i] == '.')) {
                //     numStr += expression[i++];
                // }
                while (i < expression.length() && (isdigit(expression[i]) || expression[i] == '.')) {
                    numStr += expression[i++];
                }
                double num = std::stod(numStr);
                values.push(num); // 将负数压入栈中
                --i; // 因为for循环还会自增一次
                continue; // 跳过后续处理，直接进入下一个循环
            }
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

