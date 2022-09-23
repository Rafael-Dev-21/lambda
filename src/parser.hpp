#pragma once
#include "ast.hpp"
#include "lexer.hpp"
#include <exception>
#include <memory>
#include <vector>

class ParseError : public std::exception {};

class Parser {
  std::vector<Token> tokens;
  int current = 0;

public:
  Parser(std::vector<Token> tokens) : tokens(std::move(tokens)) {}

  std::unique_ptr<Expr> parse();

private:
  std::unique_ptr<Expr> var();
  std::unique_ptr<Expr> lambda();
  std::unique_ptr<Expr> apply();
  std::unique_ptr<Expr> expr();

  bool match(int t);
  bool check(int t);
  Token advance();
  bool isatend();
  Token peek();
  Token previous();
  Token consume(int t, const std::string &msg);
  ParseError error(Token tok, const std::string &msg);
};
