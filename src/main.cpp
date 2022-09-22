#include "debug.hpp"
#include "lexer.hpp"
#include "parser.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

static void run(const std::string &source) {
    Lexer lexer(source);
    Parser parser(lexer.lex());

    AstPrinter().print(parser.parse().get());
}

static void repl() {
    std::string source;
    while (true)
    {
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
