#include "ast.hpp"
#include <unordered_map>

struct Interpreter: public ExprVisitor {

public:
    Interpreter();
    Interpreter(std::unordered_map<std::string, std::unique_ptr<Expr>> bindings);

    std::unique_ptr<Expr> eval(const Expr *expr);
    std::unique_ptr<Expr> visitVarExpr(const VarExpr *expr) override;
    std::unique_ptr<Expr> visitLambdaExpr(const LambdaExpr *expr) override;

    std::unique_ptr<Expr> visitApplyExpr(const ApplyExpr *expr) override;

private:
    std::string newname(const std::string &name) const;

    std::unordered_map<std::string, std::unique_ptr<Expr>> bindings;
};
