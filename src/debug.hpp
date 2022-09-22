#pragma once
#include <iostream>
#include <string>

#include "ast.hpp"

class AstPrinter : public ExprVisitor {
public:
  void print(Expr *expr) {
    expr->accept(this);
    std::cout << '\n';
  }

  void visitVarExpr(const VarExpr *expr) const override {
    std::cout << expr->getName();
  }

  void visitLambdaExpr(const LambdaExpr *expr) const override {
    std::cout << "$" << expr->getBinder() << ". ";
    expr->getBody()->accept((ExprVisitor *)this);
  }

  void visitApplyExpr(const ApplyExpr *expr) const override {
    std::cout << "(";
    expr->getLeft()->accept((ExprVisitor *)this);
    std::cout << ") ";
    expr->getRight()->accept((ExprVisitor *)this);
  }
};
