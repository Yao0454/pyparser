#ifndef PARSER_HPP_
#define PARSER_HPP_

#include <iostream>
#include <string>
#include <vector>

struct Line {
    std::string line;
    size_t indent;
};

enum class TokenType {
    IDENT,
    LBRAK,
    RBRAK,
    COLOM,
    EQUAL,
    ADD,
    GREATER,
    IF,
    ASSIGN,
    DEF,
    NUMBER,
    LCBRAK,
    RCBRAK,

};

struct Token {
    std::string token;
    TokenType type;
};

std::vector<std::string> parse_indent(const std::vector<Line> &lines);

std::vector<Token> parse_token(const std::vector<std::string> &lines);

#endif // PARSER_HPP_
