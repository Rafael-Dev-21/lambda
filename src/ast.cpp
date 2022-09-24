#include "ast.hpp"

std::unique_ptr<Expr> VarExpr::clone() const {
  return std::make_unique<VarExpr>(name);
}

std::string VarExpr::str() const { return name; }

std::unique_ptr<Expr> VarExpr::accept(ExprVisitor *visitor) const {
    return visitor->visitVarExpr(this);
}

std::unique_ptr<Expr> LambdaExpr::clone() const {
  return std::make_unique<LambdaExpr>(binder, body->clone());
}

std::string LambdaExpr::str() const { return "/" + binder + "." + body->str(); }

std::unique_ptr<Expr> LambdaExpr::accept(ExprVisitor *visitor) const {
    return visitor->visitLambdaExpr(this);
}

std::unique_ptr<Expr> ApplyExpr::clone() const {
  return std::make_unique<ApplyExpr>(left->clone(), right->clone());
}

std::string ApplyExpr::str() const {
  return "(" + left->str() + " " + right->str() + ")";
}

std::unique_ptr<Expr> ApplyExpr::accept(ExprVisitor *visitor) const {
    return visitor->visitApplyExpr(this);
}
