#include "../include/tool.hpp"

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
    if(expr[0] == '+' || expr[0] == '*' || expr[0] == '/' || expr[0] == '^') return false; //开头不能出现+*/^
    if(expr[expr.size() - 1] == '+' || expr[expr.size() - 1] == '*' || expr[expr.size() - 1] == '/' || expr[expr.size() - 1] == '-'  || expr[expr.size() - 1] == '^') return false; //尾部不能出现+-*/^
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
    if(op == '^') return 3; // 假设有幂运算符^，优先级最高
    return 0;
}

bool isHigherPrecedence(char op1, char op2) {
    return precedence(op1) >= precedence(op2);
}