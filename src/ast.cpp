#include "ast.hpp"

SubstCtx::ShadowGuard
SubstCtx::register_shadow(const std::string &binder) noexcept {
  shadow[binder]++;
  return ShadowGuard{this, &shadow.find(binder)->first};
}

SubstCtx::SubstGuard SubstCtx::register_subst(const std::string *from,
                                              Expr *to) noexcept {
  args.push_back({from, to});
  return SubstGuard{this};
}

std::unique_ptr<Expr>
SubstCtx::reduced_expr_for_subs(const VarExpr *expr) noexcept {
  for (auto it = args.rbegin(); it != args.rend(); it++) {
    if (expr->name == *it->from) {
      return shadow[expr->name] > 0 ? expr->clone() : it->to->reduce(this);
    }
  }

  return expr->clone();
}

std::unique_ptr<Expr> VarExpr::clone() const {
  return std::make_unique<VarExpr>(name);
}

std::string VarExpr::str() const { return name; }

std::unique_ptr<Expr> VarExpr::reduce(SubstCtx *ctx) const {
  return ctx->reduced_expr_for_subs(this);
}

std::unique_ptr<Expr> LambdaExpr::clone() const {
  return std::make_unique<LambdaExpr>(binder, body->clone());
}

std::string LambdaExpr::str() const { return "/" + binder + "." + body->str(); }

std::unique_ptr<Expr> LambdaExpr::reduce(SubstCtx *ctx) const {
  auto guard = ctx->register_shadow(binder);
  return std::make_unique<LambdaExpr>(binder, body->reduce(ctx));
}

std::unique_ptr<Expr> ApplyExpr::clone() const {
  return std::make_unique<ApplyExpr>(left->clone(), right->clone());
}

std::string ApplyExpr::str() const {
  return "(" + left->str() + " " + right->str() + ")";
}

std::unique_ptr<Expr> ApplyExpr::reduce(SubstCtx *ctx) const {
  auto resolved_left_uptr = left->reduce(ctx);
  auto resolved_left = dynamic_cast<LambdaExpr *>(resolved_left_uptr.get());

  if (resolved_left == nullptr) {
    return std::make_unique<ApplyExpr>(std::move(resolved_left_uptr),
                                       right->reduce(ctx));
  }

  auto guard = ctx->register_subst(&resolved_left->binder, right.get());
  return resolved_left->body->reduce(ctx);
}
