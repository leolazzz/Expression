#include "expression.hpp"
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <cstring>
#include <cmath>

#define RUN_TEST(test) \
    try { \
        test(); \
        std::cout << "[OK] " << #test << std::endl; \
    } catch (const std::exception& e) { \
        std::cout << "[FAIL] " << #test << " - " << e.what() << std::endl; \
    }

#define ASSERT(condition) \
    if (!(condition)) { \
        throw std::runtime_error("Assertion failed: " #condition); \
    }

void test_creation_from_string1() {
    Expression<long double> expr1("1");
    ASSERT(expr1.to_string() == std::to_string(1.0));
}

void test_creation_from_string2() {
    Expression<long double> expr1("x");
    ASSERT(expr1.to_string() == "x");
}

void test_creation_from_string3() {
    Expression<long double> expr1("1 + x");
    ASSERT(expr1.to_string() == "(" + std::to_string(1.0) + " + x)");
}

void test_creation_from_digit1() {
    Expression<long double> expr1(1.0);
    ASSERT(expr1.to_string() == std::to_string(1.0));
}

void test_creation_from_digit2() {
    Expression<long double> expr1(101.0);
    ASSERT(expr1.to_string() == std::to_string(101.0));
}

void test_copy1() {
    Expression<long double> expr1(101.0);
    Expression<long double> expr2(expr1);
    ASSERT(expr1.to_string() == expr2.to_string());
}


void test_copy2() {
    Expression<long double> expr1("1 + x + y");
    Expression<long double> expr2(expr1);
    ASSERT(expr1.to_string() == expr2.to_string());
}

void test_copy3() {
    Expression<long double> expr1("cos(1) - exp(x) + ln(y)");
    Expression<long double> expr2(expr1);
    ASSERT(expr1.to_string() == expr2.to_string());
}


void test_move1() {
    Expression<long double> expr1(101.0);
    Expression<long double> expr2(std::move(expr1));
    Expression<long double> expr3(101.0);
    ASSERT(expr3.to_string() == expr2.to_string());
}


void test_move2() {
    Expression<long double> expr1("1 + x + y");
    Expression<long double> expr2(std::move(expr1));
    Expression<long double> expr3("1 + x + y");
    ASSERT(expr3.to_string() == expr2.to_string());
}

void test_move3() {
    Expression<long double> expr1("cos(1) - exp(x) + ln(y)");
    Expression<long double> expr2(std::move(expr1));
    Expression<long double> expr3("cos(1) - exp(x) + ln(y)");
    ASSERT(expr3.to_string() == expr2.to_string());
}

void test_op_copy1() {
    Expression<long double> expr1(101.0);
    Expression<long double> expr2 = expr1;
    ASSERT(expr1.to_string() == expr2.to_string());
}


void test_op_copy2() {
    Expression<long double> expr1("1 + x + y");
    Expression<long double> expr2 = expr1;
    ASSERT(expr1.to_string() == expr2.to_string());
}

void test_op_copy3() {
    Expression<long double> expr1("cos(1) - exp(x) + ln(y)");
    Expression<long double> expr2 = expr1;
    ASSERT(expr1.to_string() == expr2.to_string());
}

void test_op_move1() {
    Expression<long double> expr1(101.0);
    Expression<long double> expr2 = std::move(expr1);
    Expression<long double> expr3(101.0);
    ASSERT(expr3.to_string() == expr2.to_string());
}

void test_op_move2() {
    Expression<long double> expr1("1 + x + y");
    Expression<long double> expr2 = std::move(expr1);
    Expression<long double> expr3("1 + x + y");
    ASSERT(expr3.to_string() == expr2.to_string());
}

void test_op_move3() {
    Expression<long double> expr1("cos(1) - exp(x) + ln(y)");
    Expression<long double> expr2 = std::move(expr1);
    Expression<long double> expr3("cos(1) - exp(x) + ln(y)");
    ASSERT(expr3.to_string() == expr2.to_string());
}

void test_add1() {
    Expression<long double> expr1("cos(1) + ln(y)");
    Expression<long double> tmp1("cos(1)");
    Expression<long double> tmp2("ln(y)");
    Expression<long double> expr2 = tmp1 + tmp2;
    ASSERT(expr1.to_string() == expr2.to_string());
}

void test_add2() {
    Expression<long double> expr1("1 + y");
    Expression<long double> tmp1("1");
    Expression<long double> tmp2("y");
    Expression<long double> expr2 = tmp1 + tmp2;
    ASSERT(expr1.to_string() == expr2.to_string());
}

void test_sub1() {
    Expression<long double> expr1("cos(1) - ln(y)");
    Expression<long double> tmp1("cos(1)");
    Expression<long double> tmp2("ln(y)");
    Expression<long double> expr2 = tmp1 - tmp2;
    ASSERT(expr1.to_string() == expr2.to_string());
}

void test_sub2() {
    Expression<long double> expr1("1 - y");
    Expression<long double> tmp1("1");
    Expression<long double> tmp2("y");
    Expression<long double> expr2 = tmp1 - tmp2;
    ASSERT(expr1.to_string() == expr2.to_string());
}

void test_mul1() {
    Expression<long double> expr1("cos(1) * ln(y)");
    Expression<long double> tmp1("cos(1)");
    Expression<long double> tmp2("ln(y)");
    Expression<long double> expr2 = tmp1 * tmp2;
    ASSERT(expr1.to_string() == expr2.to_string());
}

void test_mul2() {
    Expression<long double> expr1("1 * y");
    Expression<long double> tmp1("1");
    Expression<long double> tmp2("y");
    Expression<long double> expr2 = tmp1 * tmp2;
    ASSERT(expr1.to_string() == expr2.to_string());
}

void test_div1() {
    Expression<long double> expr1("cos(1) / ln(y)");
    Expression<long double> tmp1("cos(1)");
    Expression<long double> tmp2("ln(y)");
    Expression<long double> expr2 = tmp1 / tmp2;
    ASSERT(expr1.to_string() == expr2.to_string());
}

void test_div2() {
    Expression<long double> expr1("1 / y");
    Expression<long double> tmp1("1");
    Expression<long double> tmp2("y");
    Expression<long double> expr2 = tmp1 / tmp2;
    ASSERT(expr1.to_string() == expr2.to_string());
}

void test_pow1() {
    Expression<long double> expr1("cos(1) ^ ln(y)");
    Expression<long double> tmp1("cos(1)");
    Expression<long double> tmp2("ln(y)");
    Expression<long double> expr2 = tmp1 ^ tmp2;
    ASSERT(expr1.to_string() == expr2.to_string());
}

void test_pow2() {
    Expression<long double> expr1("1 ^ y");
    Expression<long double> tmp1("1");
    Expression<long double> tmp2("y");
    Expression<long double> expr2 = tmp1 ^ tmp2;
    ASSERT(expr1.to_string() == expr2.to_string());
}

void test_sin1() {
    Expression<long double> expr1("sin(1)");
    Expression<long double> tmp("1");
    Expression<long double> expr2 = sin(tmp);
    ASSERT(expr1.to_string() == expr2.to_string());
}

void test_sin2() {
    Expression<long double> expr1("sin(x)");
    Expression<long double> tmp("x");
    Expression<long double> expr2 = sin(tmp);
    ASSERT(expr1.to_string() == expr2.to_string());
}

void test_cos1() {
    Expression<long double> expr1("cos(1)");
    Expression<long double> tmp("1");
    Expression<long double> expr2 = cos(tmp);
    ASSERT(expr1.to_string() == expr2.to_string());
}

void test_cos2() {
    Expression<long double> expr1("cos(x)");
    Expression<long double> tmp("x");
    Expression<long double> expr2 = cos(tmp);
    ASSERT(expr1.to_string() == expr2.to_string());
}

void test_exp1() {
    Expression<long double> expr1("exp(1)");
    Expression<long double> tmp("1");
    Expression<long double> expr2 = exp(tmp);
    ASSERT(expr1.to_string() == expr2.to_string());
}

void test_exp2() {
    Expression<long double> expr1("exp(x)");
    Expression<long double> tmp("x");
    Expression<long double> expr2 = exp(tmp);
    ASSERT(expr1.to_string() == expr2.to_string());
}

void test_ln1() {
    Expression<long double> expr1("ln(1)");
    Expression<long double> tmp("1");
    Expression<long double> expr2 = ln(tmp);
    ASSERT(expr1.to_string() == expr2.to_string());
}

void test_ln2() {
    Expression<long double> expr1("ln(x)");
    Expression<long double> tmp("x");
    Expression<long double> expr2 = ln(tmp);
    ASSERT(expr1.to_string() == expr2.to_string());
}

void test_eval_val1() {
    Expression<long double> expr("1");
    std::map<std::string, long double> context;
    ASSERT(expr.eval(context) == (long double)1);
}

void test_eval_val2() {
    Expression<int> expr("123");
    std::map<std::string, int> context;
    ASSERT(expr.eval(context) == (int)123);
}

void test_to_string_val1() {
    Expression<int> expr("1");
    ASSERT(expr.to_string() == std::to_string(1));
}

void test_to_string_val2() {
    Expression<long double> expr("1");
    ASSERT(expr.to_string() == std::to_string((long double)1.0));
}

void test_diff_val1() {
    Expression<int> expr("1");
    ASSERT(expr.diff("") == std::to_string(0));
}

void test_diff_val2() {
    Expression<long double> expr("1");
    ASSERT(expr.diff("") == std::to_string((long double)0.0));
}

void test_eval_var1() {
    Expression<long double> expr("x");
    std::map<std::string, long double> context = {{"x", 1.0}};
    ASSERT(expr.eval(context) == (long double)1);
}

void test_eval_var2() {
    Expression<int> expr("abcd");
    std::map<std::string, int> context = {{"abcd", 123}};
    ASSERT(expr.eval(context) == (int)123);
}

void test_to_string_var1() {
    Expression<int> expr("x");
    ASSERT(expr.to_string() == "x");
}

void test_to_string_var2() {
    Expression<long double> expr("y");
    ASSERT(expr.to_string() == "y");
}

void test_diff_var1() {
    Expression<int> expr("x");
    ASSERT(expr.diff("x") == "1");
}

void test_diff_var2() {
    Expression<long double> expr("x");
    ASSERT(expr.diff("x") == "1");
}

void test_eval_add1() {
    Expression<long double> expr("x + y");
    std::map<std::string, long double> context = {{"x", 1.0}, {"y", 2.0}};
    ASSERT(expr.eval(context) == (long double)3);
}

void test_eval_add2() {
    Expression<int> expr("1 + 2");
    std::map<std::string, int> context;
    ASSERT(expr.eval(context) == (int)3);
}

void test_to_string_add1() {
    Expression<int> expr("x + y");
    ASSERT(expr.to_string() == "(x + y)");
}

void test_to_string_add2() {
    Expression<int> expr("1 + 2");
    ASSERT(expr.to_string() == "(1 + 2)");
}

void test_diff_add1() {
    Expression<int> expr("1 + 1");
    ASSERT(expr.diff("") == "(0 + 0)");
}

void test_diff_add2() {
    Expression<long double> expr("x + y");
    ASSERT(expr.diff("x") == "(1 + 0)");
}

void test_eval_sub1() {
    Expression<long double> expr("x - y");
    std::map<std::string, long double> context = {{"x", 2.0}, {"y", 1.0}};
    ASSERT(expr.eval(context) == (long double)1);
}

void test_eval_sub2() {
    Expression<int> expr("10 - 2");
    std::map<std::string, int> context;
    ASSERT(expr.eval(context) == (int)8);
}

void test_to_string_sub1() {
    Expression<int> expr("x - y");
    ASSERT(expr.to_string() == "(x - y)");
}

void test_to_string_sub2() {
    Expression<int> expr("1 - 2");
    ASSERT(expr.to_string() == "(1 - 2)");
}

void test_diff_sub1() {
    Expression<int> expr("1 - 1");
    ASSERT(expr.diff("") == "(0 - 0)");
}

void test_diff_sub2() {
    Expression<long double> expr("x - y");
    ASSERT(expr.diff("x") == "(1 - 0)");
}

void test_eval_mul1() {
    Expression<long double> expr("x * y");
    std::map<std::string, long double> context = {{"x", 2.0}, {"y", 3.0}};
    ASSERT(expr.eval(context) == (long double)6);
}

void test_eval_mul2() {
    Expression<int> expr("10 * 2");
    std::map<std::string, int> context;
    ASSERT(expr.eval(context) == (int)20);
}

void test_to_string_mul1() {
    Expression<int> expr("x * y");
    ASSERT(expr.to_string() == "(x * y)");
}

void test_to_string_mul2() {
    Expression<int> expr("1 * 2");
    ASSERT(expr.to_string() == "(1 * 2)");
}

void test_diff_mul1() {
    Expression<int> expr("1 * 1");
    ASSERT(expr.diff("") == "(1 * 0 + 0 * 1)");
}

void test_diff_mul2() {
    Expression<long double> expr("x * y");
    ASSERT(expr.diff("x") == "(x * 0 + 1 * y)");
}

void test_eval_div1() {
    Expression<long double> expr("x / y");
    std::map<std::string, long double> context = {{"x", 2.0}, {"y", 1.0}};
    ASSERT(expr.eval(context) == (long double)2.0);
}

void test_eval_div2() {
    Expression<int> expr("10 / 2");
    std::map<std::string, int> context;
    ASSERT(expr.eval(context) == (int)5);
}

void test_to_string_div1() {
    Expression<int> expr("x / y");
    ASSERT(expr.to_string() == "(x / y)");
}

void test_to_string_div2() {
    Expression<int> expr("1 / 2");
    ASSERT(expr.to_string() == "(1 / 2)");
}

void test_diff_div1() {
    Expression<int> expr("1 / 1");
    ASSERT(expr.diff("") == "((0 * 1 - 1 * 0) / (1 ^ 2))");
}

void test_diff_div2() {
    Expression<long double> expr("x / y");
    ASSERT(expr.diff("x") == "((1 * y - x * 0) / (y ^ 2))");
}

void test_eval_pow1() {
    Expression<long double> expr("x ^ y");
    std::map<std::string, long double> context = {{"x", 2.0}, {"y", 1.0}};
    ASSERT(expr.eval(context) == (long double)2.0);
}

void test_eval_pow2() {
    Expression<int> expr("10 ^ 2");
    std::map<std::string, int> context;
    ASSERT(expr.eval(context) == (int)100);
}

void test_to_string_pow1() {
    Expression<int> expr("x ^ y");
    ASSERT(expr.to_string() == "(x ^ y)");
}

void test_to_string_pow2() {
    Expression<int> expr("1 ^ 2");
    ASSERT(expr.to_string() == "(1 ^ 2)");
}

void test_diff_pow1() {
    Expression<int> expr("1 ^ 1");
    ASSERT(expr.diff("") == "(1 * (1 ^ (1 - 1)) * 0)");
}

void test_diff_pow2() {
    Expression<int> expr("x ^ y");
    ASSERT(expr.diff("x") == "(y * (x ^ (y - 1)) * 1)");
}

void test_eval_sin1() {
    Expression<long double> expr("sin(x)");
    std::map<std::string, long double> context = {{"x", 2.0}};
    ASSERT(expr.eval(context) == std::sin((long double)2.0));
}

void test_eval_sin2() {
    Expression<long double> expr("sin(2)");
    std::map<std::string, long double> context;
    ASSERT(expr.eval(context) == std::sin((long double)2.0));
}

void test_to_string_sin1() {
    Expression<int> expr("sin(x)");
    ASSERT(expr.to_string() == "sin(x)");
}

void test_to_string_sin2() {
    Expression<int> expr("sin(1)");
    ASSERT(expr.to_string() == "sin(1)");
}

void test_diff_sin1() {
    Expression<int> expr("sin(1)");
    ASSERT(expr.diff("") == "(cos(1) * 0)");
}

void test_diff_sin2() {
    Expression<long double> expr("sin(x)");
    ASSERT(expr.diff("x") == "(cos(x) * 1)");
}

void test_eval_cos1() {
    Expression<long double> expr("cos(x)");
    std::map<std::string, long double> context = {{"x", 2.0}};
    ASSERT(expr.eval(context) == std::cos((long double)2.0));
}

void test_eval_cos2() {
    Expression<long double> expr("cos(2)");
    std::map<std::string, long double> context;
    ASSERT(expr.eval(context) == std::cos((long double)2.0));
}

void test_to_string_cos1() {
    Expression<int> expr("cos(x)");
    ASSERT(expr.to_string() == "cos(x)");
}

void test_to_string_cos2() {
    Expression<int> expr("cos(1)");
    ASSERT(expr.to_string() == "cos(1)");
}

void test_diff_cos1() {
    Expression<int> expr("cos(1)");
    ASSERT(expr.diff("") == "(-sin(1) * 0)");
}

void test_diff_cos2() {
    Expression<long double> expr("cos(x)");
    ASSERT(expr.diff("x") == "(-sin(x) * 1)");
}

void test_eval_ln1() {
    Expression<long double> expr("ln(x)");
    std::map<std::string, long double> context = {{"x", 2.0}};
    ASSERT(expr.eval(context) == std::log((long double)2.0));
}

void test_eval_ln2() {
    Expression<long double> expr("ln(2)");
    std::map<std::string, long double> context;
    ASSERT(expr.eval(context) == std::log((long double)2.0));
}

void test_to_string_ln1() {
    Expression<int> expr("ln(x)");
    ASSERT(expr.to_string() == "ln(x)");
}

void test_to_string_ln2() {
    Expression<int> expr("ln(1)");
    ASSERT(expr.to_string() == "ln(1)");
}

void test_diff_ln1() {
    Expression<int> expr("ln(1)");
    ASSERT(expr.diff("") == "(0/1)");
}

void test_diff_ln2() {
    Expression<long double> expr("ln(x)");
    ASSERT(expr.diff("x") == "(1/x)");
}

void test_eval_exp1() {
    Expression<long double> expr("exp(x)");
    std::map<std::string, long double> context = {{"x", 2.0}};
    ASSERT(expr.eval(context) == std::exp((long double)2.0));
}

void test_eval_exp2() {
    Expression<long double> expr("exp(2)");
    std::map<std::string, long double> context;
    ASSERT(expr.eval(context) == std::exp((long double)2.0));
}

void test_to_string_exp1() {
    Expression<int> expr("exp(x)");
    ASSERT(expr.to_string() == "exp(x)");
}

void test_to_string_exp2() {
    Expression<int> expr("exp(1)");
    ASSERT(expr.to_string() == "exp(1)");
}

void test_diff_exp1() {
    Expression<int> expr("exp(1)");
    ASSERT(expr.diff("") == "(exp(1) * 0)");
}

void test_diff_exp2() {
    Expression<long double> expr("exp(x)");
    ASSERT(expr.diff("x") == "(exp(x) * 1)");
}

int main() {
    RUN_TEST(test_creation_from_string1);
    RUN_TEST(test_creation_from_string2);
    RUN_TEST(test_creation_from_string3);

    RUN_TEST(test_creation_from_digit1);
    RUN_TEST(test_creation_from_digit2);

    RUN_TEST(test_copy1);
    RUN_TEST(test_copy2);
    RUN_TEST(test_copy3);

    RUN_TEST(test_move1);
    RUN_TEST(test_move2);
    RUN_TEST(test_move3);

    RUN_TEST(test_op_copy1);
    RUN_TEST(test_op_copy2);
    RUN_TEST(test_op_copy3);

    RUN_TEST(test_op_move1);
    RUN_TEST(test_op_move2);
    RUN_TEST(test_op_move3);

    RUN_TEST(test_add1);
    RUN_TEST(test_add2);
    RUN_TEST(test_eval_add1);
    RUN_TEST(test_eval_add2);
    RUN_TEST(test_diff_add1);
    RUN_TEST(test_diff_add2);
    RUN_TEST(test_to_string_add1);
    RUN_TEST(test_to_string_add2);

    RUN_TEST(test_sub1);
    RUN_TEST(test_sub2);
    RUN_TEST(test_eval_sub1);
    RUN_TEST(test_eval_sub2);
    RUN_TEST(test_diff_sub1);
    RUN_TEST(test_diff_sub2);
    RUN_TEST(test_to_string_sub1);
    RUN_TEST(test_to_string_sub2);

    RUN_TEST(test_mul1);
    RUN_TEST(test_mul2);
    RUN_TEST(test_eval_mul1);
    RUN_TEST(test_eval_mul2);
    RUN_TEST(test_diff_mul1);
    RUN_TEST(test_diff_mul2);
    RUN_TEST(test_to_string_mul1);
    RUN_TEST(test_to_string_mul2);

    RUN_TEST(test_div1);
    RUN_TEST(test_div2);
    RUN_TEST(test_eval_div1);
    RUN_TEST(test_eval_div2);
    RUN_TEST(test_diff_div1);
    RUN_TEST(test_diff_div2);
    RUN_TEST(test_to_string_div1);
    RUN_TEST(test_to_string_div2);

    RUN_TEST(test_pow1);
    RUN_TEST(test_pow2);
    RUN_TEST(test_eval_pow1);
    RUN_TEST(test_eval_pow2);
    RUN_TEST(test_diff_pow1);
    RUN_TEST(test_diff_pow2);
    RUN_TEST(test_to_string_pow1);
    RUN_TEST(test_to_string_pow2);

    RUN_TEST(test_sin1);
    RUN_TEST(test_sin2);
    RUN_TEST(test_eval_sin1);
    RUN_TEST(test_eval_sin2);
    RUN_TEST(test_diff_sin1);
    RUN_TEST(test_diff_sin2);
    RUN_TEST(test_to_string_sin1);
    RUN_TEST(test_to_string_sin2);

    RUN_TEST(test_cos1);
    RUN_TEST(test_cos2);
    RUN_TEST(test_eval_cos1);
    RUN_TEST(test_eval_cos2);
    RUN_TEST(test_diff_cos1);
    RUN_TEST(test_diff_cos2);
    RUN_TEST(test_to_string_cos1);
    RUN_TEST(test_to_string_cos2);

    RUN_TEST(test_ln1);
    RUN_TEST(test_ln2);
    RUN_TEST(test_eval_ln1);
    RUN_TEST(test_eval_ln2);
    RUN_TEST(test_diff_ln1);
    RUN_TEST(test_diff_ln2);
    RUN_TEST(test_to_string_ln1);
    RUN_TEST(test_to_string_ln2);

    RUN_TEST(test_exp1);
    RUN_TEST(test_exp2);
    RUN_TEST(test_eval_exp1);
    RUN_TEST(test_eval_exp2);
    RUN_TEST(test_diff_exp1);
    RUN_TEST(test_diff_exp2);
    RUN_TEST(test_to_string_exp1);
    RUN_TEST(test_to_string_exp2);
}