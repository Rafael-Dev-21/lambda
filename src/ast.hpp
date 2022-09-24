#pragma once
#include <memory>
#include <string>



struct ExprVisitor;

struct Expr {

public:
  virtual ~Expr() = default;
  virtual std::unique_ptr<Expr> clone() const = 0;
  virtual std::string str() const = 0;
  virtual std::unique_ptr<Expr> accept(ExprVisitor *visitor) const = 0;
};

struct VarExpr : public Expr {
  std::string name;

public:
  VarExpr(const std::string &name) : name(name) {}
  std::unique_ptr<Expr> clone() const override;
  std::string str() const override;
  std::unique_ptr<Expr> accept(ExprVisitor *visitor) const override;
};

struct LambdaExpr : public Expr {
  std::string binder;
  std::unique_ptr<Expr> body;

public:
  LambdaExpr(const std::string &binder, std::unique_ptr<Expr> body)
      : binder(binder), body(std::move(body)) {}
  std::unique_ptr<Expr> clone() const override;
  std::string str() const override;
  std::unique_ptr<Expr> accept(ExprVisitor *visitor) const override;
};

struct ApplyExpr : public Expr {
  std::unique_ptr<Expr> left;
  std::unique_ptr<Expr> right;

public:
  ApplyExpr(std::unique_ptr<Expr> left, std::unique_ptr<Expr> right)
      : left(std::move(left)), right(std::move(right)) {}
  std::unique_ptr<Expr> clone() const override;
  std::string str() const override;
  std::unique_ptr<Expr> accept(ExprVisitor *visitor) const override;
};


struct ExprVisitor {
public:
    ~ExprVisitor() = default;
    virtual std::unique_ptr<Expr> visitVarExpr(const VarExpr *expr) = 0;
    virtual std::unique_ptr<Expr> visitLambdaExpr(const LambdaExpr *expr) = 0;
    virtual std::unique_ptr<Expr> visitApplyExpr(const ApplyExpr *expr) = 0;
};
