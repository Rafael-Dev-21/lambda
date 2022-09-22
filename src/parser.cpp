#include "parser.hpp"
#include "log.hpp"

std::unique_ptr<Expr> Parser::parse() {
    auto t = apply();

    if (!t)
        return nullptr;

    return t;
}

std::unique_ptr<Expr> Parser::var() {
    return std::make_unique<VarExpr>(previous().name);
}

std::unique_ptr<Expr> Parser::paren() {
    auto v = apply();
    if (!v) {
        console::error(peek(), "Expected expression.");
        return nullptr;
    }
    if (!consume(')', "Expected ')' after expression."))
        return nullptr;
    return v;
}

std::unique_ptr<Expr> Parser::lambda() {
    std::string binder = advance().name;
    if (!consume('.', " Expected '.' after binder."))
        return nullptr;
    auto body = apply();
    if (!body) {
        console::error(peek(), "Expected body.");
        return nullptr;
    }
    return std::make_unique<LambdaExpr>(binder, std::move(body));
}

std::unique_ptr<Expr> Parser::apply() {
    auto left = expr();
    if (!left)
    {
        console::error(peek(), "Expected expression");
        return nullptr;
    }

    while (true) {
        if (match(')') || match(tok_eof))
            return left;

        auto right = expr();
        if (!right)
            return left;

        left = std::make_unique<ApplyExpr>(std::move(left), std::move(right));
    }
}

std::unique_ptr<Expr> Parser::expr() {
    if (match(tok_lambda))
        return lambda();
    if (match('('))
        return apply();
    if (match(tok_symbol))
        return var();
    if (match(tok_eof))
        console::error(peek(), "Expected expression.");

    return nullptr;
}

bool Parser::match(int t) {
    if (check(t)) {
        advance();
        return true;
    }
    return false;
}

bool Parser::check(int t) {
    if (isatend()) return false;
    return peek().type == t;
}

Token Parser::advance() {
    if (!isatend()) current++;
    return previous();
}

bool Parser::isatend() {
    return peek().type == tok_eof;
}

Token Parser::peek() {
    return tokens[current];
}

Token Parser::previous() {
    return tokens[current - 1];
}

bool Parser::consume(int t, const std::string &msg) {
    if (check(t)) {
        advance();
        return true;
    }

    console::error(peek(), msg);
    return false;
}
