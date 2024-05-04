#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <string>
int choseoperator = 4;  //选择运算符

// 生成一个随机数
double generateRandomNumber(int max, bool useDecimal) {
    if (useDecimal) {
        return (double)(rand() % max + 1) / (rand() % 10 + 1);
    }
    else {
        return rand() % max + 1;
    }
}

// 生成一个随机运算符
char generateRandomOperator(int choseoperator) {
    char operators[] = { '+', '-', '*', '/' };
    if (choseoperator == 0) {
        return operators[0];
    }
    else if (choseoperator == 1) {
        return operators[1];
    }
    else if (choseoperator == 2) {
        return operators[2];
    }
    else if (choseoperator == 3) {
        return operators[3];
    }
    else {
        return operators[rand() % 4];
    }  
}

//生成四则运算
std::string generateQuestion(int max, bool useBrackets, bool useDecimal, int length) {
    std::stringstream ss;
    for (int i = 0; i < length - 1; i++) {
        double num = generateRandomNumber(max, useDecimal);
        double num1 = generateRandomNumber(max, useDecimal);
        char op = generateRandomOperator(choseoperator);
        char op1 = generateRandomOperator(choseoperator);
        if (useBrackets) {
            ss << "(";
        }

        ss << num << " " << op << " " << num1 << " ";

        if (useBrackets) {
            ss << ")" <<  " " << op1 << " ";
        }
        else {
            ss << op1 << " ";
        }
    }
    // 添加最后一个数字
    double num = generateRandomNumber(max, useDecimal);
    ss << num << '\n';

    return ss.str();
}

int main() {
    srand(time(0));  // 初始化随机数生成器

    int max = 10;  // 最大数
    int numQuestions = 5;  // 题目数量
    bool useBrackets = true;  // 是否使用括号
    bool useDecimal = true;  // 是否使用小数
    int length = 3;  // 四则运算式子的长度

    std::cout << "请输入最大数：";
    std::cin >> max;

    std::cout << "请输入需要生成的题目数量：";
    std::cin >> numQuestions;

    std::cout << "请选择运算符(0加法,1减法,2乘法,3除法,4随机)：";
    std::cin >> choseoperator;

    std::cout << "请输入生成题目的长度：";
    std::cin >> length;

    std::cout << "是否使用括号(0不使用或1使用)：";
    std::cin >> useBrackets;

    std::cout << "是否使用小数(0不使用或1使用)：";
    std::cin >> useDecimal;

    std::cout << "选择questions.txt文件生成路径：";
    std::string path;
    std::cin >> path;
    std::ofstream file(path);  // 创建一个文件

    // 生成题目并写入文件
    for (int i = 0; i < numQuestions; i++) {
        std::string question = generateQuestion(max, useBrackets, useDecimal, length);
        file << question << std::endl;
    }

    file.close();  // 关闭文件

    std::cout << "文件questions.txt位于" << path << std::endl;

    return 0;
}

