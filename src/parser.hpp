#pragma once
#include "ast.hpp"
#include "lexer.hpp"
#include <memory>
#include <vector>

class Parser {
  std::vector<Token> tokens;
  int current = 0;

public:
  Parser(std::vector<Token> tokens) : tokens(std::move(tokens)) {}

  std::unique_ptr<Expr> parse();

private:
  std::unique_ptr<Expr> var();
  std::unique_ptr<Expr> paren();
  std::unique_ptr<Expr> lambda();
  std::unique_ptr<Expr> apply();
  std::unique_ptr<Expr> expr();

  bool match(int t);
  bool check(int t);
  Token advance();
  bool isatend();
  Token peek();
  Token previous();
  bool consume(int t, const std::string &msg);
};
