#pragma once
#include <memory>
#include <string>

class ExprVisitor;

class Expr {

public:
  virtual ~Expr() = default;
  virtual void accept(ExprVisitor *visitor) const = 0;
};

class VarExpr : public Expr {
  std::string name;

public:
  VarExpr(const std::string &name) : name(name) {}

  std::string getName() const { return name; }

  void accept(ExprVisitor *visitor) const override;
};

class LambdaExpr : public Expr {
  std::string binder;
  std::unique_ptr<Expr> body;

public:
  LambdaExpr(const std::string &binder, std::unique_ptr<Expr> body)
      : binder(binder), body(std::move(body)) {}

  std::string getBinder() const { return binder; }

  Expr *getBody() const { return body.get(); }

  void accept(ExprVisitor *visitor) const override;
};

class ApplyExpr : public Expr {
  std::unique_ptr<Expr> left;
  std::unique_ptr<Expr> right;

public:
  ApplyExpr(std::unique_ptr<Expr> left, std::unique_ptr<Expr> right)
      : left(std::move(left)), right(std::move(right)) {}

  Expr *getLeft() const { return left.get(); }

  Expr *getRight() const { return right.get(); }

  void accept(ExprVisitor *visitor) const override;
};

class ExprVisitor {
public:
  ~ExprVisitor() = default;
  virtual void visitVarExpr(const VarExpr *expr) const = 0;
  virtual void visitLambdaExpr(const LambdaExpr *expr) const = 0;
  virtual void visitApplyExpr(const ApplyExpr *expr) const = 0;
};
