#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

struct SubstCtx;

struct Expr {

public:
  virtual ~Expr() = default;
  virtual std::unique_ptr<Expr> clone() const = 0;
  virtual std::string str() const = 0;
  virtual std::unique_ptr<Expr> reduce(SubstCtx *ctx) const = 0;
};

struct VarExpr : public Expr {
  std::string name;

public:
  VarExpr(const std::string &name) : name(name) {}
  std::unique_ptr<Expr> clone() const override;
  std::string str() const override;
  std::unique_ptr<Expr> reduce(SubstCtx *ctx) const override;
};

struct LambdaExpr : public Expr {
  std::string binder;
  std::unique_ptr<Expr> body;

public:
  LambdaExpr(const std::string &binder, std::unique_ptr<Expr> body)
      : binder(binder), body(std::move(body)) {}
  std::unique_ptr<Expr> clone() const override;
  std::string str() const override;
  std::unique_ptr<Expr> reduce(SubstCtx *ctx) const override;
};

struct ApplyExpr : public Expr {
  std::unique_ptr<Expr> left;
  std::unique_ptr<Expr> right;

public:
  ApplyExpr(std::unique_ptr<Expr> left, std::unique_ptr<Expr> right)
      : left(std::move(left)), right(std::move(right)) {}
  std::unique_ptr<Expr> clone() const override;
  std::string str() const override;
  std::unique_ptr<Expr> reduce(SubstCtx *ctx) const override;
};

struct SubstCtx {
private:
  struct SubstOp {
    const std::string *from;
    const Expr *to;
  };

  std::vector<SubstOp> args;
  std::unordered_map<std::string, std::size_t> shadow;

public:
  struct ShadowGuard {
    SubstCtx *ctx;
    const std::string *binder;

    ~ShadowGuard() { ctx->shadow[*binder]--; };
  };

  struct SubstGuard {
    SubstCtx *ctx;

    ~SubstGuard() { ctx->args.pop_back(); }
  };

  ShadowGuard register_shadow(const std::string &binder) noexcept;
  SubstGuard register_subst(const std::string *from, Expr *to) noexcept;
  std::unique_ptr<Expr> reduced_expr_for_subs(const VarExpr *expr) noexcept;
};
