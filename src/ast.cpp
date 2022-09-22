#include "ast.hpp"

void VarExpr::accept(ExprVisitor *visitor) const {
  visitor->visitVarExpr(this);
}

void LambdaExpr::accept(ExprVisitor *visitor) const {
  visitor->visitLambdaExpr(this);
}

void ApplyExpr::accept(ExprVisitor *visitor) const {
  visitor->visitApplyExpr(this);
}
