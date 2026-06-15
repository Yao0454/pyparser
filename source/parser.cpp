#include "../include/parser.hpp"
#include <csignal>
#include <cstddef>
#include <iostream>
#include <stdexcept>
#include <vector>

Token peek(const std::vector<Token> &tokens, size_t pos) { return tokens[pos]; }

Token advance(const std::vector<Token> &tokens, size_t pos) { return tokens[pos++]; }

void expect(const std::vector<Token> &tokens, TokenType t, size_t pos) {
    if (tokens[pos].type != t) {
        std::cerr << "Wrong token type\n";
        throw std::runtime_error("Wrong token type`");
    }
    pos += 1;
}
