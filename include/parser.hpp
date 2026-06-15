#ifndef PARSER_HPP_
#define PARSER_HPP_

#include "lexer.hpp"
#include <cstddef>
#include <vector>

Token peek(const std::vector<Token> &tokens, size_t pos);
Token advance(const std::vector<Token> &tokens, size_t &pos);
void expect(const std::vector<Token> &tokens, size_t &pos, TokenType t);

void parse_primary(const std::vector<Token> &tokens, size_t &pos);
void parse_additive(const std::vector<Token> &tokens, size_t &pos);
void parse_expression(const std::vector<Token> &tokens, size_t &pos);
void parse_statement(const std::vector<Token> &tokens, size_t &pos);
void parse_block(const std::vector<Token> &tokens, size_t &pos);

#endif // PARSER_HPP_
