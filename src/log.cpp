#include "log.hpp"

namespace console {
    void report(int line, int col, const std::string &where, const std::string &msg) {
        std::cerr << "[line " << line << "; col " << col << "] Error" << where << ": " << msg << "\n";
    }

    void error(int line, int col, const std::string &msg) {
        report(line, col, "", msg);
    }

    void error(Token tok, const std::string &msg) {
        if (tok.type == tok_eof) {
            report(tok.line, tok.column, " at end", msg);
        } else {
            report(tok.line, tok.column, " at '" + tok.name + "'", msg);
        }
    }

    void logi(const std::string &tag, const std::string &msg) {
        std::cout << "[" << tag << "] " << msg << "\n";
    }
}
