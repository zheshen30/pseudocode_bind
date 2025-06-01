#include<iostream>
#include<iomanip>
#include "src_t.h"
void print(int a, double b, const std::string& c) {
    std::cout << a << " " << b << " " << c << std::endl; // 打印三个参数到控制台
}

// 测试类 - 用于演示绑定成员函数的功能
struct Calculator
{
    // 成员函数 - 将两个数相加并格式化输出结果
    double add(int a, int b)
    {
        double result = a + b;
        std::cout << std::fixed << std::setprecision(2) << result << std::endl;
        return result;
    }

    // const成员函数 - 将两个数相乘并格式化输出结果
    double multiply(int a, int b) const
    {
        double result = a * b;
        std::cout << std::fixed << std::setprecision(2) << result << std::endl;
        return result;
    }
};

int main() {
    // 测试用例1
    auto f1 = my_bind(print, _1, 3.14, _2); 
    f1(100, "hello");  

    // 测试用例2
    auto f2 = my_bind(print, _3, _2, _1); 
    f2("world", 2.718, 200); 

    // 测试用例3
    auto f3 = my_bind(print, 999, _1, "fixed"); 
    f3(1.618);  

    Calculator calc;
    const  Calculator ccalc;
    // 测试用例4
    auto f4 = my_bind(&Calculator::add, &calc, _1, _2);
    f4(2, 3); 

    // 测试用例5
    auto f5 = my_bind(&Calculator::multiply, &calc, 10, _1);
    f5(5); 

    // 测试用例6
    auto f6 = my_bind(&Calculator::multiply, Calculator(), _1, 100);
    f6(50);  

    // 测试用例7
    auto f7 = my_bind(&Calculator::multiply, ccalc, 10, _1);
    f7(5);  // 调用calc.multiply(10, 5)，输出: 50.00

    // 测试用例8
    auto f8 = my_bind(&Calculator::multiply, &ccalc, _1, _2);
    f8(2, 3); 

    auto f9 = my_bind(&Calculator::add,calc, _1, _2);
    f9(2, 3);  

     //测试用例9
    auto f10 = my_bind(&Calculator::multiply, calc, 10, _1);
    f10(5);  

    return 0; 
}
