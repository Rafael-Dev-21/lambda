#include "lexer.hpp"
#include "parser.hpp"
#include "interpreter.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

static void run(const std::string &source) {
  Lexer lexer(source);
  Parser parser(lexer.lex());
  Interpreter interpreter;
  auto result = parser.parse();
  auto expr = dynamic_cast<Expr *>(result.get());

  if (expr == nullptr) {
    cerr << "Failed parsing.";
    return;
  }

  cout << expr->str() << "\n";
  cout << interpreter.eval(expr)->str() << "\n";
}

static void repl() {
  std::string source;
  while (true) {
    cout << "calλ> ";
    getline(cin, source);
    run(source);
  }
}

static void runFile(const std::string &path) {
  ifstream file(path);
  stringstream buffer;
  buffer << file.rdbuf();
  run(buffer.str());
}

int main(int argc, char **argv) {

  if (argc > 2) {
    cerr << "Usage: " << argv[0] << " [file]\n";
  } else if (argc == 2) {
    runFile(argv[1]);
  } else {
    repl();
  }

  return 0;
}
