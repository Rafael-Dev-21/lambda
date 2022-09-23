#pragma once
#include "lexer.hpp"
#include <iostream>

namespace console {
void logi(const std::string &tag, const std::string &msg);

void error(int line, int col, const std::string &msg);

void error(Token tok, const std::string &msg);
} // namespace console
