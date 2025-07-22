#include "../include/calculate.hpp"

//计算表达式函数
Calculate::Result Calculate::eval(std::string& expression)
{
    removeSpaces(expression);//先移除字符串中的空白字符
    std::stack<double> values; //存储数字
    std::stack<char> ops; //存储运算符
    std::stack<std::string> funNames; //存储函数名
    std::vector<std::string> tokens = strToTokens(expression); //将字符串分割成tokens
    for(size_t i = 0; i < tokens.size(); ++i) {
        const std::string& token = tokens[i];
        if (isdigit(token[0]) || (token.size() > 1 && token[0] == '-' && isdigit(token[1]))) {
            // 如果是数字，直接将其转换为double并压入栈中
            try {
                double value = std::stod(token);
                values.push(value);
            } catch (const std::invalid_argument& e) {
                return Result(0.0, "无效的数字格式: " + token, true);
            } catch (const std::out_of_range& e) {
                return Result(0.0, "数字超出范围: " + token, true);
            }
        } else if (token == "+" || token == "-" || token == "*" || token == "/" || token == "^") {
            // 如果是运算符，处理运算符栈
            while (!ops.empty() && precedence(ops.top()) >= precedence(token[0])) {
                char op = ops.top();
                ops.pop();
                if (values.size() < 2) {
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
                        return Result(0.0, "程序员，酒吧，炒面.jpg", true);
                }
                values.push(result);
            }
            ops.push(token[0]);
        }
        else if(isFuncation(token)) {
            funNames.push(token);  // 需要额外栈存储函数名
        }
        else if (token == "(") {
            ops.push('(');
        } 
        else if (token == ")") {
            while (!ops.empty() && ops.top() != '(') {
                char op = ops.top();
                ops.pop();
                if (values.size() < 2) {
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
                        return Result(0.0, "程序员，酒吧，炒面.jpg", true);
                }
                values.push(result);
            }
            if (ops.empty() || ops.top() != '(') {
                return Result(0.0, "括号不匹配", true);
            }
            if(!funNames.empty()) {
                std::string funName = funNames.top();
                funNames.pop(); // 弹出函数名
                if(values.empty()) {
                    return Result(0.0, "函数调用缺少参数", true);
                }
                double arg = values.top(); values.pop();
                if(functions.find(funName) != functions.end()) {
                    double result = functions[funName](arg);
                    values.push(result);
                }
                else {
                    return Result(0.0, "未知函数: " + funName, true);
                }
            } else {
                funNames.pop(); // 弹出函数名
            }
            ops.pop(); // 弹出左括号
        }
        else if(isFuncation(token)) {
            //如果是预定义函数，先检查括号是否匹配
            if(i + 1 < tokens.size() && tokens[i + 1] == "(") {
                i++;//跳过左括号
                if(values.empty()){
                    return Result(0.0, "函数调用缺少参数", true);
                }
                double arg = values.top(); values.pop();
                if(functions.find(token) != functions.end()) {
                    double result = functions[token](arg);
                    values.push(result);
                }
            }
        }
        else {
            return Result(0.0, "无效的令牌: " + token, true);
        }
    }
    // 处理剩余的运算符
    while (!ops.empty()) {
        char op = ops.top();
        ops.pop();
        if (values.size() < 2) {
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
                return Result(0.0, "程序员，酒吧，炒面.jpg", true);
        }
        values.push(result);
    }
    if (values.size() == 1) {
        return Result(values.top());
    } else {
        return Result(0.0, "表达式错误：结果异常", true);
    }
}
    // 下面是原来的代码，暂时注释掉
    // for (size_t i = 0; i < expression.length(); ++i) {
    //     // 如果是数字，提取完整的数字
    //     if (isdigit(expression[i]) || expression[i] == '.') {
    //         std::string numStr;
    //         while (i < expression.length() && (isdigit(expression[i]) || expression[i] == '.')) {
    //             numStr += expression[i++];
    //         }
    //         double num = std::stod(numStr);
    //         values.push(num);
    //         --i; // 因为for循环还会自增一次
    //     }
    //     // 如果是四则运算符
    //     else if (expression[i] == '+' || expression[i] == '-' || expression[i] == '*' || expression[i] == '/') {
    //         // 检查是否是负数
    //         if(expression[i] == '-' && (i == 0 || expression[i-1] == '(' )) {
    //             // 这是负号，和后面的数字一起解析
    //             std::string numStr;
    //             numStr += expression[i++]; //此时直接将-压入字符串中了，所以后面不需要压入-num了
    //             while (i < expression.length() && (isdigit(expression[i]) || expression[i] == '.')) {
    //                 numStr += expression[i++];
    //             }
    //             double num = std::stod(numStr);
    //             values.push(num); // 将负数压入栈中
    //             --i; // 因为for循环还会自增一次
    //             continue; // 跳过后续处理，直接进入下一个循环
    //         }
    //         // 处理栈顶运算符的优先级
    //         while(!ops.empty() && ops.top() != '(' && isHigherPrecedence(ops.top(), expression[i])) {
    //             char op = ops.top();
    //             ops.pop();
    //             if(values.size() < 2) {
    //                 return Result(0.0, "表达式错误：运算数字数量不足", true);
    //             }
    //             double b = values.top(); values.pop();
    //             double a = values.top(); values.pop();
    //             double result = 0.0;
    //             switch(op) {
    //                 case '+': result = a + b; break;
    //                 case '-': result = a - b; break;
    //                 case '*': result = a * b; break;
    //                 case '/':
    //                     if(b == 0) {
    //                         return Result(0.0, "谁教你的让0当除数(╯▔皿▔)╯", true);
    //                     }
    //                     result = a / b;
    //                     break;
    //                 case '^': result = std::pow(a, b); break;
    //                 default:
    //                     return Result(0.0, "1程序员，酒吧，炒面.jpg", true);
    //             }
    //             values.push(result);
    //         }
    //         ops.push(expression[i]);
    //     }
    //     // 如果是幂运算符
    //     else if(expression[i] == '^') {
    //         // 处理幂运算符
    //         while(!ops.empty() && ops.top() != '(' && precedence(ops.top()) > precedence(expression[i])) {
    //             char op = ops.top();
    //             ops.pop();
    //             if (values.size() < 2) {
    //                 return Result(0.0, "表达式错误，运算数字不足", true);
    //             }
    //             double b = values.top(); values.pop();
    //             double a = values.top(); values.pop();
    //             double result = std::pow(a, b); // 使用cmath库的pow函数计算幂
    //             values.push(result);
    //         }
    //         ops.push(expression[i]);
    //     }
    //     // 如果是括号
    //     else if (expression[i] == '(') {
    //         ops.push(expression[i]);
    //     }
    //     // 如果是右括号
    //     else if (expression[i] == ')') {
    //         if(ops.empty()) {
    //             return Result(0.0, "Bro忘记输左括号了吧", true);
    //         }
    //         while(ops.top() !='(')
    //         {
    //             char op = ops.top();
    //             ops.pop();
    //             if(values.size() < 2) {
    //                 return Result(0.0, "表达式错误：运算数字数量不足", true);
    //             }
    //             double b = values.top(); values.pop();
    //             double a = values.top(); values.pop();
    //             double result = 0.0;
    //             switch(op) {
    //                 case '+': result = a + b; break;
    //                 case '-': result = a - b; break;
    //                 case '*': result = a * b; break;
    //                 case '/':
    //                     if(b == 0) {
    //                         return Result(0.0, "谁教你的让0当除数(╯▔皿▔)╯", true);
    //                     }
    //                     result = a / b;
    //                     break;
    //                 case '^': result = std::pow(a, b); break;
    //                 default:
    //                     return Result(0.0, "程序员，11酒吧，炒面.jpg", true);
    //             }
    //             values.push(result);
    //         }
    //         ops.pop(); // 弹出左括号
    //     }
    //     else {
    //         return Result(0.0, "程序员，111酒吧，炒面.jpg", true);
    //     }
    // }
    // // 剩余运算符
    // while(!ops.empty()) {
    //     char op = ops.top();
    //     ops.pop();
    //     if(values.size() < 2) {
    //         return Result(0.0, "表达式错误：运算数字数量不足", true);
    //     }
    //     double b = values.top(); values.pop();
    //     double a = values.top(); values.pop();
    //     double result = 0.0;
    //     switch(op) {
    //         case '+': result = a + b; break;
    //         case '-': result = a - b; break;
    //         case '*': result = a * b; break;
    //         case '/':
    //             if(b == 0) {
    //                 return Result(0.0, "谁教你的让0当除数(╯▔皿▔)╯", true);
    //             }
    //             result = a / b;
    //             break;
    //         case '^': result = std::pow(a, b); break; // 处理幂运算
    //         default:
    //             return Result(0.0, "1111程序员，酒吧，炒面.jpg", true);
    //     }
    //     values.push(result);
    // }
    // if (values.size() == 1)
    //     {return Result(values.top());}
    // else
    //     return Result(0.0, "表达式错误：结果异常", true);