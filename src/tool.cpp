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

// 判断是否是预定义函数(如三角函数)
bool isFuncation(const std::string& token) {
    static const std::unordered_set<std::string> functions = {"sin", "cos", "tan", "log", "sqrt"};
    return functions.find(token) != functions.end();
}

// 判断运算符的优先级 若op1的优先级小于等于op2则返回true
int precedence(char op) {
    if(isFuncation(std::string(1, op))) return 4; // 假设函数的优先级最高
    if(op == '+' || op == '-') return 1;
    if(op == '*' || op == '/') return 2;
    if(op == '^') return 3; // 假设有幂运算符^，优先级最高
    return 0;
}

bool isHigherPrecedence(char op1, char op2) {
    return precedence(op1) >= precedence(op2);
}

std::vector<std::string> strToTokens(std::string& str){
    std::vector<std::string> tokens;
    std::string token;
    std::unordered_set<std::string> functions = {"sin", "cos", "tan", "log", "sqrt"};
    //先清除所有空格
    std::string strNospace = removeSpaces(str);
    for (size_t i = 0; i < strNospace.length(); ++i) {
        // 如果是数字，提取完整的数字
        if (isdigit(strNospace[i]) || strNospace[i] == '.') {
            std::string numStr;
            while (i < strNospace.length() && (isdigit(strNospace[i]) || strNospace[i] == '.')) {
                numStr += strNospace[i++];
            }
            tokens.push_back(numStr);
            --i; // 因为for循环还会自增一次
        }
        //如果是四则运算符
        else if (strNospace[i] == '+' || strNospace[i] == '-' || strNospace[i] == '*' || strNospace[i] == '/' || strNospace[i] == '^' || strNospace[i] == '(' || strNospace[i] == ')') {
            //处理负数
            if(strNospace[i] == '-' && (i == 0 || strNospace[i-1] == '(')) {
                std::string numStr;
                numStr += strNospace[i++];
                while(i < strNospace.length() && (isdigit(strNospace[i]) || strNospace[i] == '.')) {
                    numStr += strNospace[i++];
                }
                tokens.push_back(numStr);
            }
            tokens.push_back(std::string(1, strNospace[i]));
        }
        //如果是字母
        else if (isalpha(strNospace[i])) {
            std::string name;
            while (i < strNospace.length() && (isalnum(strNospace[i]) || strNospace[i] == '_')) {
                name += strNospace[i++]; // 支持字母、数字、下划线（如 x1, my_var）
            }
            
            // 检查是否是预定义函数
            if (functions.find(name) != functions.end()) {
                // 函数名后必须跟括号（如 sin(30)）
                if (i >= strNospace.length() || strNospace[i] != '(') {
                    throw std::runtime_error("函数名 " + name + " 后缺少括号");
                }
                tokens.push_back(name);
            } else {
                // 非函数名当作变量处理（或报错，根据需求）
                tokens.push_back(name); 
            }
            --i; // 修正索引
        }
    }
    return tokens;
}

