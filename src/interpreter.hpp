#include "ast.hpp"
#include <unordered_map>

struct Interpreter: public ExprVisitor {

public:
    Interpreter() {}
    Interpreter(std::unordered_map<std::string, std::unique_ptr<Expr>> bindings)
        : bindings(std::move(bindings)) {}

    std::unique_ptr<Expr> eval(const Expr *expr) {
        return expr->accept(this);
    }

    std::unique_ptr<Expr> visitVarExpr(const VarExpr *expr) override {
        if (bindings[expr->name]) {
            return bindings[expr->name]->clone();
        }
        return expr->clone();
    }

    std::unique_ptr<Expr> visitLambdaExpr(const LambdaExpr *expr) override {
        if (bindings[expr->binder]) {
            const std::string &collided = expr->binder;
            const std::string renamed = newname(collided);
            std::unique_ptr<Expr> renamed_var = std::make_unique<VarExpr>(renamed);

            std::unordered_map<std::string, std::unique_ptr<Expr>> rename_table;
            rename_table[collided] = std::move(renamed_var);

            Interpreter renamer(std::move(rename_table));
            std::unique_ptr<Expr> renamed_body = expr->body->accept(&renamer);

            return std::make_unique<LambdaExpr>(renamed, std::move(renamed_body));
        } else {
            return std::make_unique<LambdaExpr>(expr->binder, expr->body->accept(this));
        }
    }

    std::unique_ptr<Expr> visitApplyExpr(const ApplyExpr *expr) override {
        auto left_uptr = expr->left->accept(this);
        auto left = dynamic_cast<LambdaExpr *>(left_uptr.get());

        if (left == nullptr) {
            return std::make_unique<ApplyExpr>(
                    std::move(left_uptr),
                    expr->right->accept(this)
                    );
        }

        std::string binder = left->binder;

        bindings[binder] = expr->right->clone();
        return left->body->accept(this);
    }

private:
    std::string newname(const std::string &name) const {
        std::string renamed = name;

        do {
            renamed += "'";
        } while (bindings.find(renamed) != bindings.end());

        return renamed;
    }

    std::unordered_map<std::string, std::unique_ptr<Expr>> bindings;
};
