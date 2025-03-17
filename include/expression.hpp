#pragma once
#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include <string>
#include <map>
#include <memory>

template<typename T>
class ExpressionImpl {
public:
    virtual ~ExpressionImpl() = default;
    virtual T eval(std::map<std::string, T> context) const = 0;
    virtual std::string to_string() const = 0;
    virtual std::string diff(std::string var) const = 0;
};

template<typename T>
class Expression {
public:
    Expression(std::string variable);
    Expression(T value);
    Expression(const Expression& copy);
    Expression(Expression&& moved);
    Expression() = default;
    ~Expression() = default;

    Expression<T>& operator= (const Expression<T>& that);
    Expression<T>& operator=(Expression&& that);
    Expression<T> operator+ (const Expression<T>& that);
    Expression<T>& operator+=(const Expression<T>& that);
    Expression<T> operator- (const Expression<T>& that);
    Expression<T>& operator-=(const Expression<T>& that);
    Expression<T> operator* (const Expression<T>& that);
    Expression<T>& operator*=(const Expression<T>& that);
    Expression<T> operator/ (const Expression<T>& that);
    Expression<T>& operator/=(const Expression<T>& that);
    Expression<T> operator^ (const Expression<T>& that);
    Expression<T>& operator^=(const Expression<T>& that);

    template<typename V>
    friend Expression<V> sin(Expression<V> that);
    template<typename V>
    friend Expression<V> cos(Expression<V> that);
    template<typename V>
    friend Expression<V> ln(Expression<V> that);
    template<typename V>
    friend Expression<V> exp(Expression<V> that);

    T eval(std::map<std::string, T> context) const;
    std::string to_string() const;
    std::string diff(std::string var) const;
private:
    Expression(std::shared_ptr<ExpressionImpl<T>> impl);
    std::shared_ptr<ExpressionImpl<T>> impl_;
};

template<typename T>
class Value : public ExpressionImpl<T> {
public:
    Value(T value);
    virtual ~Value() override = default;

    virtual T eval(std::map<std::string, T> context) const override;
    virtual std::string to_string() const override;
    virtual std::string diff(std::string var) const override ;
private:
    T value_;
};

template<typename T>
class Variable : public ExpressionImpl<T> {
public:
    Variable(std::string value);
    virtual ~Variable() override = default;

    virtual T eval(std::map<std::string, T> context) const override;
    virtual std::string to_string() const override;
    virtual std::string diff(std::string var) const override;
private:
    std::string name_;
};

template<typename T>
class OperationAdd : public ExpressionImpl<T> {
public:
    OperationAdd(Expression<T> left, Expression<T> right);
    virtual ~OperationAdd() override = default;

    virtual T eval(std::map<std::string, T> context) const override;
    virtual std::string to_string() const override;
    virtual std::string diff(std::string var) const override;
private:
    Expression<T> left_;
    Expression<T> right_;
};

template<typename T>
class OperationSub : public ExpressionImpl<T> {
public:
    OperationSub(Expression<T> left, Expression<T> right);
    virtual ~OperationSub() override = default;

    virtual T eval(std::map<std::string, T> context) const override;
    virtual std::string to_string() const override;
    virtual std::string diff(std::string var) const override;
private:
    Expression<T> left_;
    Expression<T> right_;
};

template<typename T>
class OperationMul : public ExpressionImpl<T> {
public:
    OperationMul(Expression<T> left, Expression<T> right);
    virtual ~OperationMul() override = default;

    virtual T eval(std::map<std::string, T> context) const override;
    virtual std::string to_string() const override;
    virtual std::string diff(std::string var) const override;
private:
    Expression<T> left_;
    Expression<T> right_;
};

template<typename T>
class OperationDiv : public ExpressionImpl<T> {
public:
    OperationDiv(Expression<T> left, Expression<T> right);
    virtual ~OperationDiv() override = default;

    virtual T eval(std::map<std::string, T> context) const override;
    virtual std::string to_string() const override;
    virtual std::string diff(std::string var) const override;
private:
    Expression<T> left_;
    Expression<T> right_;
};

template<typename T>
class OperationPow : public ExpressionImpl<T> {
public:
    OperationPow(Expression<T> left, Expression<T> right);
    virtual ~OperationPow() override = default;

    virtual T eval(std::map<std::string, T> context) const override;
    virtual std::string to_string() const override;
    virtual std::string diff(std::string var) const override;
private:
    Expression<T> left_;
    Expression<T> right_;
};

template<typename T>
class OperationSin : public ExpressionImpl<T> {
public:
    OperationSin(Expression<T> expr);
    virtual ~OperationSin() override = default;

    virtual T eval(std::map<std::string, T> context) const override;
    virtual std::string to_string() const override;
    virtual std::string diff(std::string var) const override;
private:
    Expression<T> expr_;
};

template<typename T>
class OperationCos : public ExpressionImpl<T> {
public:
    OperationCos(Expression<T> expr);
    virtual ~OperationCos() override = default;

    virtual T eval(std::map<std::string, T> context) const override;
    virtual std::string to_string() const override;
    virtual std::string diff(std::string var) const override;
private:
    Expression<T> expr_;
};

template<typename T>
class OperationLn : public ExpressionImpl<T> {
public:
    OperationLn(Expression<T> expr);
    virtual ~OperationLn() override = default;

    virtual T eval(std::map<std::string, T> context) const override;
    virtual std::string to_string() const override;
    virtual std::string diff(std::string var) const override;
private:
    Expression<T> expr_;
};

template<typename T>
class OperationExp : public ExpressionImpl<T> {
public:
    OperationExp(Expression<T> expr);
    virtual ~OperationExp() override = default;

    virtual T eval(std::map<std::string, T> context) const override;
    virtual std::string to_string() const override;
    virtual std::string diff(std::string var) const override;
private:
    Expression<T> expr_;
};

#endif