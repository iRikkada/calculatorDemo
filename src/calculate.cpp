#include "../include/calculate.hpp"

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
                    case '^': result = std::pow(a, b); break;
                    default:
                        return Result(0.0, "1程序员，酒吧，炒面.jpg", true);
                }
                values.push(result);
            }
            ops.push(expression[i]);
        }
        // 如果是幂运算符
        else if(expression[i] == '^') {
            // 处理幂运算符
            while(!ops.empty() && ops.top() != '(' && precedence(ops.top()) > precedence(expression[i])) {
                char op = ops.top();
                ops.pop();
                if (values.size() < 2) {
                    return Result(0.0, "表达式错误，运算数字不足", true);
                }
                double b = values.top(); values.pop();
                double a = values.top(); values.pop();
                double result = std::pow(a, b); // 使用cmath库的pow函数计算幂
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
                    case '^': result = std::pow(a, b); break;
                    default:
                        return Result(0.0, "程序员，11酒吧，炒面.jpg", true);
                }
                values.push(result);
            }
            ops.pop(); // 弹出左括号
        }
        else {
            return Result(0.0, "程序员，111酒吧，炒面.jpg", true);
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
            case '^': result = std::pow(a, b); break; // 处理幂运算
            default:
                return Result(0.0, "1111程序员，酒吧，炒面.jpg", true);
        }
        values.push(result);
    }
    if (values.size() == 1)
        {return Result(values.top());}
    else
        return Result(0.0, "表达式错误：结果异常", true);
}