#include "expression.hpp"
#include <stdexcept>
#include <iostream>
#include <complex>
#include <typeinfo>
#include <cmath>
#include <string>
#include <map>
#include <memory>

template<typename T>
Expression<T>::Expression(std::shared_ptr<ExpressionImpl<T>> impl) : impl_(impl) {}

template<typename T>
Expression<T>::Expression(T val) : impl_(std::make_shared<Value<T>>(val)) {}

template<typename T>
Expression<T>::Expression(const Expression& copy) : impl_(std::shared_ptr<ExpressionImpl<T>>(copy.impl_)) {}

template<typename T>
Expression<T>::Expression(Expression&& moved) : impl_(std::move(moved.impl_)) {
    moved.impl_ = nullptr;
}

template<typename T>
Expression<T>& Expression<T>::operator= (const Expression<T>& that) {
    if (this == &that) {
        return *this;
    }
    impl_ = that.impl_;
    return *this;
}

template<typename T>
Expression<T>& Expression<T>::operator= (Expression<T>&& that) {
    if (this == &that) {
        return *this;
    }
    impl_ = std::move(that.impl_);
    that.impl_ = nullptr;
    return *this;
}

template<typename T>
Expression<T> Expression<T>::operator+ (const Expression<T>& that) {
    return Expression<T>(std::make_shared<OperationAdd<T>>(*this, that));
}

template<typename T>
Expression<T>& Expression<T>::operator+=(const Expression<T>& that) {
    *this = *this + that;
    return *this;
}

template<typename T>
Expression<T> Expression<T>::operator- (const Expression<T>& that) {
    return Expression<T>(std::make_shared<OperationSub<T>>(*this, that));
}

template<typename T>
Expression<T>& Expression<T>::operator-=(const Expression<T>& that) {
    *this = *this - that;
    return *this;
}

template<typename T>
Expression<T> Expression<T>::operator* (const Expression<T>& that) {
    return Expression<T>(std::make_shared<OperationMul<T>>(*this, that));
}

template<typename T>
Expression<T>& Expression<T>::operator*=(const Expression<T>& that) {
    *this = *this * that;
    return *this;
}

template<typename T>
Expression<T> Expression<T>::operator/ (const Expression<T>& that) {
    return Expression<T>(std::make_shared<OperationDiv<T>>(*this, that));
}

template<typename T>
Expression<T>& Expression<T>::operator/=(const Expression<T>& that) {
    *this = *this / that;
    return *this;
}

template<typename T>
Expression<T> Expression<T>::operator^ (const Expression<T>& that) {
    return Expression<T>(std::make_shared<OperationPow<T>>(*this, that));
}

template<typename T>
Expression<T>& Expression<T>::operator^=(const Expression<T>& that) {
    *this = *this ^ that;
    return *this;
}

template<typename T>
T Expression<T>::eval(std::map<std::string, T> context) const {
    return impl_->eval(context);
}

template<typename T>
std::string Expression<T>::to_string() const {
    return impl_->to_string();
}

template<typename T>
std::string Expression<T>::diff(std::string var) const {
    return impl_->diff(var);
}

template<typename V>
Expression<V> sin(Expression<V> expr) {
    return Expression<V>(std::make_shared<OperationSin<V>>(OperationSin<V>(expr)));
}

template<typename V>
Expression<V> cos(Expression<V> expr) {
    return Expression<V>(std::make_shared<OperationCos<V>>(OperationCos<V>(expr)));
}

template<typename V>
Expression<V> ln(Expression<V> expr) {
    return Expression<V>(std::make_shared<OperationLn<V>>(expr));
}

template<typename V>
Expression<V> exp(Expression<V> expr) {
    return Expression<V>(std::make_shared<OperationExp<V>>(expr));
}

template<typename T>
Value<T>::Value(T value) : value_(value) {}

template<typename T>
T Value<T>::eval(std::map<std::string, T> context) const {
    return value_;
}

template<typename T>
struct is_std_complex_helper : std::false_type {};

template<typename T>
struct is_std_complex_helper<std::complex<T>> : std::true_type {};

template<typename T>
struct is_std_complex : is_std_complex_helper<std::remove_cv_t<std::remove_reference_t<T>>> {};

template<typename T>
std::string Value<T>::to_string() const {
    if constexpr (is_std_complex<T>::value){
        return "(" + std::to_string(value_.real())  + "+" + std::to_string(value_.imag()) + "i" + ")";
    } else{
        return std::to_string(value_);
    }
}

template<typename T>
std::string Value<T>::diff(std::string var) const {
    return "0";
}

template<typename T>
Variable<T>::Variable(std::string name) : name_(name) {}

template<typename T>
T Variable<T>::eval(std::map<std::string, T> context) const {
    auto iter = context.find(name_);
    if (iter == context.end()) { 
        throw("The variable \"" + name_ + "\" is undefined\n");
    }
    return iter->second;
}

template<typename T>
std::string Variable<T>::to_string() const {
    return name_;
}

template<typename T>
std::string Variable<T>::diff(std::string var) const {   
    if(var != name_){
        return "0";
    }
    return "1";
}

template<typename T>
OperationAdd<T>::OperationAdd(Expression<T> left, Expression<T> right) : left_  (left), right_ (right) {}

template<typename T>
T OperationAdd<T>::eval(std::map<std::string, T> context) const {
    T value_left  = left_.eval(context);
    T value_right = right_.eval(context);
    return value_left + value_right;
}

template<typename T>
std::string OperationAdd<T>::to_string() const {
    return "("   + left_.to_string()  + " + " + right_.to_string() + ")";
}

template<typename T>
std::string OperationAdd<T>::diff(std::string var) const {   
    return "("   + left_.diff(var)  + " + " + right_.diff(var) + ")";
}

template<typename T>
OperationSub<T>::OperationSub(Expression<T> left, Expression<T> right) : left_  (left), right_ (right) {}

template<typename T>
T OperationSub<T>::eval(std::map<std::string, T> context) const {
    return left_.eval(context) - right_.eval(context);
}

template<typename T>
std::string OperationSub<T>::to_string() const {
    return "("   + left_.to_string()  + " - " + right_.to_string() + ")";
}

template<typename T>
std::string OperationSub<T>::diff(std::string var) const {
    return "("   + left_.diff(var)  + " - " + right_.diff(var) + ")";
}

template<typename T>
OperationMul<T>::OperationMul(Expression<T> left, Expression<T> right) : left_  (left), right_ (right) {}

template<typename T>
T OperationMul<T>::eval(std::map<std::string, T> context) const {
    return left_.eval(context) * right_.eval(context);
}

template<typename T>
std::string OperationMul<T>::to_string() const {
    return "("   + left_.to_string()  + " * " + right_.to_string() + ")";
}

template<typename T>
std::string OperationMul<T>::diff(std::string var) const {
    return "("   + left_.to_string() + " * "  + right_.diff(var)   +
           " + " + left_.diff(var)  + " * " + right_.to_string()  + ")";
}

template<typename T>
OperationDiv<T>::OperationDiv(Expression<T> left, Expression<T> right) : left_  (left), right_ (right) {}

template<typename T>
T OperationDiv<T>::eval(std::map<std::string, T> context) const {
    T r = right_.eval(context);
    if(r == 0) {
        throw("Division by zero");
    }
    return left_.eval(context) / right_.eval(context);
}

template<typename T>
std::string OperationDiv<T>::to_string() const {
    return "("   + left_.to_string()  + " / " + right_.to_string() + ")";
}

template<typename T>
std::string OperationDiv<T>::diff(std::string var) const {   
    return "((" + left_.diff(var) + " * "  + right_.to_string()   +
           " - " + left_.to_string()  + " * " + right_.diff(var)  +
           ") / (" + right_.to_string() +  " ^ 2))";
}

template<typename T>
OperationPow<T>::OperationPow(Expression<T> left, Expression<T> right) : left_  (left), right_ (right) {}

template<typename T>
T OperationPow<T>::eval(std::map<std::string, T> context) const {
    return std::pow(left_.eval(context), right_.eval(context));
}

template<typename T>
std::string OperationPow<T>::to_string() const {
    return "("   + left_.to_string()  + " ^ " + right_.to_string() + ")";
}

template<typename T>
std::string OperationPow<T>::diff(std::string var) const {   
    Expression nw = right_;
    nw = nw - Expression<T>(1.0);
    return "(" + right_.to_string() +  " * (" + left_.to_string() + 
           " ^ " + nw.to_string() + ") * " + left_.diff(var) + ")";
}

template<typename T>
OperationSin<T>::OperationSin(Expression<T> variable) : expr_  (variable) {}

template<typename T>
T OperationSin<T>::eval(std::map<std::string, T> context) const {
    return std::sin(expr_.eval(context));
}

template<typename T>
std::string OperationSin<T>::to_string() const {
    return "sin(" + expr_.to_string()  + ")";
}

template<typename T>
std::string OperationSin<T>::diff(std::string var) const {   
    return "(cos(" + expr_.to_string() + ") * " + expr_.diff(var)+ ")";
}

template<typename T>
OperationCos<T>::OperationCos(Expression<T> variable) : expr_  (variable) {}

template<typename T>
T OperationCos<T>::eval(std::map<std::string, T> context) const {
    return std::cos(expr_.eval(context));
}

template<typename T>
std::string OperationCos<T>::to_string() const {
    return "cos(" + expr_.to_string()  + ")";
}

template<typename T>
std::string OperationCos<T>::diff(std::string var) const {   
    return "(-sin(" + expr_.to_string() + ") * " + expr_.diff(var)+ ")";
}

template<typename T>
OperationExp<T>::OperationExp(Expression<T> variable) : expr_  (variable) {}

template<typename T>
T OperationExp<T>::eval(std::map<std::string, T> context) const {
    return std::exp(expr_.eval(context));
}

template<typename T>
std::string OperationExp<T>::to_string() const {
    return "exp(" + expr_.to_string()  + ")";
}

template<typename T>
std::string OperationExp<T>::diff(std::string var) const {   
    return "(exp(" + expr_.to_string() + ") * " + expr_.diff(var) + ")";
}

template<typename T>
OperationLn<T>::OperationLn(Expression<T> variable) : expr_  (variable) {}

template<typename T>
T OperationLn<T>::eval(std::map<std::string, T> context) const {
    return std::log(expr_.eval(context));
}

template<typename T>
std::string OperationLn<T>::to_string() const {
    return "ln(" + expr_.to_string() + ")";
}

template<typename T>
std::string OperationLn<T>::diff(std::string var) const {   
    return "(" + expr_.diff(var) + "/" + expr_.to_string() + ")";
}

template<typename T>
Expression<T>::Expression(std::string variable) {
    while(variable.find(' ') < variable.size()){
        variable.replace(variable.find(' '), 1, "");
    }
    int depth = 0;
    for(int i = 0; i < variable.size(); ++i){
        if(variable[i] == '('){
            depth++;
            continue;
        }
        if(variable[i] == ')'){
            depth--;
            continue;
        }
        if(depth != 0){
            continue;
        }
        if(variable[i] == '+'){
            std::string l = variable.substr(0, i);
            if(l.size() == 0){
                l = "0";
            }
            std::string r = variable.substr(i + 1);
            impl_ = std::make_shared<OperationAdd<T>>(OperationAdd<T>(Expression<T>(l), Expression<T>(r)));
            return;
        }
        if(variable[i] == '-'){
            std::string l = variable.substr(0, i);
            if(l.size() == 0){
                l = "0";
            }
            std::string r = variable.substr(i + 1);
            impl_ = std::make_shared<OperationSub<T>>(OperationSub<T>(Expression<T>(l), Expression<T>(r)));
            return;
        }
    }
    if(depth != 0){
        throw("parenthesis missmatch");
    }
    for(int i = 0; i < variable.size(); ++i){
        if(variable[i] == '('){
            depth++;
            continue;
        }
        if(variable[i] == ')'){
            depth--;
            continue;
        }
        if(depth != 0){
            continue;
        }
        if(variable[i] == '*'){
            std::string l = variable.substr(0, i);
            std::string r = variable.substr(i + 1);
            impl_ = std::make_shared<OperationMul<T>>(OperationMul<T>(Expression<T>(l), Expression<T>(r)));
            return;
        }
        if(variable[i] == '/'){
            std::string l = variable.substr(0, i);
            std::string r = variable.substr(i + 1);
            impl_ = std::make_shared<OperationDiv<T>>(OperationDiv<T>(Expression<T>(l), Expression<T>(r)));
            return;
        }
    }

    for(int i = 0; i < variable.size(); ++i){
        if(variable[i] == '('){
            depth++;
            continue;
        }
        if(variable[i] == ')'){
            depth--;
            continue;
        }
        if(depth != 0){
            continue;
        }
        if(variable[i] == '^'){
            std::string l = variable.substr(0, i);
            std::string r = variable.substr(i + 1);
            impl_ = std::make_shared<OperationPow<T>>(OperationPow<T>(Expression<T>(l), Expression<T>(r)));
            return;
        }
    }
    
    if(variable[0] == '('){
        if(variable[variable.size() - 1] != ')'){
            throw("parenthesis missmatch");
        } else {
            std::string r = variable.substr(1, variable.size() - 2);
            Expression<T> qwe = Expression<T>(r);
            qwe = qwe - qwe;
            impl_ = std::make_shared<OperationAdd<T>>(OperationAdd<T>(qwe, Expression<T>(r)));
            return;
        }
    }

    if(variable.substr(0, 4) == "sin("){
        std::string r = variable.substr(4, variable.size() - 5);
        impl_ = std::make_shared<OperationSin<T>>(OperationSin<T>(r));
        return;
    }

    if(variable.substr(0, 4) == "cos("){
        std::string r = variable.substr(4, variable.size() - 5);
        impl_ = std::make_shared<OperationCos<T>>(OperationCos<T>(r));
        return;
    }

    if(variable.substr(0, 4) == "exp("){
        std::string r = variable.substr(4, variable.size() - 5);
        impl_ = std::make_shared<OperationExp<T>>(OperationExp<T>(r));
        return;
    }

    if(variable.substr(0, 3) == "ln("){
        std::string r = variable.substr(3, variable.size() - 4);
        impl_ = std::make_shared<OperationLn<T>>(OperationLn<T>(r));
        return;
    }

    if(variable[0] >= '0' && variable[0] <= '9'){
        if constexpr (is_std_complex<T>::value){
            if(variable[variable.size() - 1] == 'i'){
                variable.pop_back();
                T r(0, (double)std::stod(variable));
                impl_ = std::make_shared<Value<T>>(r);
                return;
            }
            T r((double)std::stod(variable), 0);
            impl_ = std::make_shared<Value<T>>(r);
            return;
        }
        T r = std::stod(variable);
        impl_ = std::make_shared<Value<T>>(r);
        return;
    }
    impl_ = std::make_shared<Variable<T>>(variable);
}
template class Expression<long double>;
template class Expression<int>;
template Expression<long double> sin<long double>(Expression<long double>);
template Expression<long double> cos<long double>(Expression<long double>);
template Expression<long double> exp<long double>(Expression<long double>);
template Expression<long double> ln<long double>(Expression<long double>);
template Expression<int> sin<int>(Expression<int>);
template Expression<int> cos<int>(Expression<int>);
template Expression<int> exp<int>(Expression<int>);
template Expression<int> ln<int>(Expression<int>);