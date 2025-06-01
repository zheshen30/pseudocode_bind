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
    // 测试用例1：基本绑定 - 参数按顺序一一对应
    auto f1 = my_bind(print, _1, 3.14, _2); // 绑定print函数，第1个参数使用占位符_1，第2个参数固定为3.14，第3个参数使用占位符_2
    f1(100, "hello");  // 调用时传入两个参数，100对应_1，"hello"对应_2，最终输出: 100 3.14 hello

    // 测试用例2：参数重排序 - 改变参数传递顺序
    auto f2 = my_bind(print, _3, _2, _1); // 绑定print函数，使用占位符改变参数顺序
    f2("world", 2.718, 200);  // 调用时，200(第3个参数)被传给a，2.718(第2个参数)被传给b，"world"(第1个参数)被传给c，输出: 200 2.718 world

    // 测试用例3：混合绑定 - 部分参数固定，部分使用占位符
    auto f3 = my_bind(print, 999, _1, "fixed"); // 绑定print函数，第1个参数固定为999，第2个参数使用占位符，第3个参数固定为"fixed"
    f3(1.618);  // 调用时只需传入一个参数给占位符_1，输出: 999 1.618 fixed

    // 创建Calculator对象
    Calculator calc;
    const  Calculator ccalc;
    // 测试用例4：绑定非静态成员函数 - 通过对象引用
    // 第一个参数是成员函数指针，第二个参数是对象指针或引用（this指针）
    auto f4 = my_bind(&Calculator::add, &calc, _1, _2);
    f4(2, 3);  // 调用calc.add(2, 3)，输出: 5.00,辅助函数1

    // 测试用例5：绑定const成员函数 - 通过对象引用
    auto f5 = my_bind(&Calculator::multiply, &calc, 10, _1);
    f5(5);  // 调用calc.multiply(10, 5)，输出: 50.00，辅助函数5

    // 测试用例6：绑定非静态成员函数 - 通过对象值
    // 创建一个新的Calculator对象，直接存储在绑定器中
    auto f6 = my_bind(&Calculator::multiply, Calculator(), _1, 100);
    f6(50);  // 调用临时Calculator对象的add(50, 100)，输出: 5000.00，辅助函数8

    // 测试用例7：绑定const成员函数 - 通过对象引用
    auto f7 = my_bind(&Calculator::multiply, ccalc, 10, _1);
    f7(5);  // 调用calc.multiply(10, 5)，输出: 50.00

    // 测试用例4：绑定非静态成员函数 - 通过对象引用
    // 第一个参数是成员函数指针，第二个参数是对象指针或引用（this指针）
    auto f8 = my_bind(&Calculator::multiply, &ccalc, _1, _2);
    f8(2, 3);  // 调用calc.add(2, 3)，输出: 5.00，辅助函数6

    auto f9 = my_bind(&Calculator::add,calc, _1, _2);
    f9(2, 3);  // 调用calc.add(2, 3)，输出: 5.00


     //测试用例5：绑定const成员函数 - 通过对象引用
    auto f10 = my_bind(&Calculator::multiply, calc, 10, _1);
    f10(5);  // 调用calc.multiply(10, 5)，输出: 50.00

    return 0; // 程序正常结束
}