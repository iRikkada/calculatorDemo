#ifndef TOOL_H
#define TOOL_H
#include<iostream>
#include<algorithm>
#include<string>
#include<cctype>
#include<limits>
#include<stack>
#include<sstream>
#include<cmath>
#include<vector>
#include<unordered_set>

//检验括号的匹配性
bool checkBrackets(const std::string& expr);
//检验操作符和操作数的位置
bool checkOperators(const std::string& expr);
//检验字符的合法性
bool checkVariables(const std::string& expr);
//综合检验
bool isValidInfix(const std::string& expr);
//移除所有空白字符
std::string removeSpaces(std::string& str);
// 判断运算符的优先级 若op1的优先级小于等于op2则返回true
int precedence(char op);
// 判断运算符的优先级
bool isHigherPrecedence(char op1, char op2);
// 将字符串分割成tokens 
std::vector<std::string> strToTokens(std::string& str);
// 判断是否是预定义函数(如三角函数)
bool isFuncation(const std::string& token);

#endif 