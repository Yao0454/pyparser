#ifndef LEXER_HPP_
#define LEXER_HPP_

#include <iostream>
#include <string>
#include <vector>

struct Line {
    std::string text;
    size_t indent;
};

enum class TokenType {
    IDENT,
    LPAREN,
    RPAREN,
    COLON,
    PLUS,
    MINUS,
    GREATER,
    LESS,
    IF,
    ASSIGN,
    DEF,
    NUMBER,
    LBRACE,
    RBRACE,
    STRING,
    END,
};

struct Token {
    std::string value;
    TokenType type;
};

std::vector<std::string> mark_indents(const std::vector<Line> &lines);

std::vector<Token> tokenize(const std::vector<std::string> &lines);

#endif // LEXER_HPP_
