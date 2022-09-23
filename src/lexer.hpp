#pragma once
#include <cstdio>
#include <string>
#include <vector>

enum TokenType {
  tok_eof = -1,
  tok_symbol = -2,
  tok_lambda = -3,
};

struct Token {
  int type;
  std::string name;
  int line;
  int column;

public:
  Token(int type, const std::string &name, int line, int column)
      : type(type), name(name), line(line), column(column) {}
};

class Lexer {
  std::string source;
  std::vector<Token> tokens;
  int start = 0;
  int current = 0;
  int line = 1;
  int column = 0;

public:
  Lexer(const std::string &source) : source(source) {}

  std::vector<Token> lex();

private:
  bool isatend();
  void scantok();
  void symbol();
  bool match(int t);
  int peek();
  int advance();
  void addtok(int t);
};
