#ifndef PARSER_HPP_
#define PARSER_HPP_

#include "lexer.hpp"
#include <cstddef>
#include <vector>

Token peek(const std::vector<Token> &tokens, size_t pos);
Token advance(const std::vector<Token> &tokens, size_t pos);
void expect(const std::vector<Token> &tokens, TokenType t, size_t pos);

#endif // PARSER_HPP_
