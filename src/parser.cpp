#include "parser.hpp"
#include "log.hpp"

std::unique_ptr<Expr> Parser::parse() { return apply(); }

std::unique_ptr<Expr> Parser::var() {
  return std::make_unique<VarExpr>(previous().name);
}

std::unique_ptr<Expr> Parser::lambda() {
  std::string binder = advance().name;
  consume('.', " Expected '.' after binder.");
  auto body = apply();

  return std::make_unique<LambdaExpr>(binder, std::move(body));
}

std::unique_ptr<Expr> Parser::apply() {
  auto left = expr();

  while (true) {
    if (check(')'))
      return left;

    auto right = expr();
    if (!right)
      return left;

    left = std::make_unique<ApplyExpr>(std::move(left), std::move(right));

    if (match(tok_eof))
      return left;
  }
}

std::unique_ptr<Expr> Parser::expr() {
  switch (advance().type) {
  default:
    return nullptr;
  case tok_lambda:
    return lambda();
  case tok_symbol:
    return var();
  case tok_eof:
    throw error(peek(), "Expected expression.");
  case '(': {
    auto a = apply();
    consume(')', "Expected ')' after expression.");
    return a;
  }
  }
}

bool Parser::match(int t) {
  if (check(t)) {
    advance();
    return true;
  }
  return false;
}

bool Parser::check(int t) {
  if (isatend())
    return false;
  return peek().type == t;
}

Token Parser::advance() {
  if (!isatend())
    current++;
  return previous();
}

bool Parser::isatend() { return peek().type == tok_eof; }

Token Parser::peek() { return tokens[current]; }

Token Parser::previous() { return tokens[current - 1]; }

Token Parser::consume(int t, const std::string &msg) {
  if (check(t))
    return advance();

  throw error(peek(), msg);
}

ParseError Parser::error(Token tok, const std::string &msg) {
  console::error(tok, msg);
  return ParseError{};
}
