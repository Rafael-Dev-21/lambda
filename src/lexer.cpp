#include "lexer.hpp"
#include "log.hpp"
#include <iostream>

std::vector<Token> Lexer::lex() {
    while (!isatend()) {
        start = current;
        scantok();
    }

    tokens.push_back(Token{tok_eof, "", line, column});
    return tokens;
}

void Lexer::scantok() {
    int t = advance();

    switch(t) {
    case '/':
        addtok(tok_lambda);
        break;
    case '.':
    case '(':
    case ')':
        addtok(t);
        break;
    case ' ':
    case '\t':
    case '\r':
        break;
    case '\n':
        line++;
        column = 0;
        break;
    default:
        if (isalnum(t))
            symbol();
        else
            console::error(line, column, "Unexpected character.");
        break;
    }
}

void Lexer::symbol() {
    while (isalnum(peek())) advance();

    addtok(tok_symbol);
}

bool Lexer::match(int t) {
    if (isatend())
        return false;
    if (source[current] != t)
        return false;

    column++;
    current++;
    return true;
}

int Lexer::peek() {
    if (isatend())
        return '\0';
    return source[current];
}

bool Lexer::isatend() {
    return current >= source.size();
}

int Lexer::advance() {
    column++;
    return source[current++];
}

void Lexer::addtok(int t) {
    std::string name = source.substr(start, current - start);
    tokens.push_back(Token{t, name, line, column});
}
