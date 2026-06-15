#include "parser.hpp"
#include "lexer.hpp"
#include <cassert>
#include <cstddef>
#include <iostream>
#include <stdexcept>
#include <vector>

Token peek(const std::vector<Token> &tokens, size_t pos) { return tokens[pos]; }

Token advance(const std::vector<Token> &tokens, size_t &pos) { return tokens[pos++]; }

void expect(const std::vector<Token> &tokens, size_t &pos, TokenType t) {
    if (tokens[pos].type != t) {
        std::cerr << "Wrong token type\n";
        throw std::runtime_error("Wrong token type`");
    }
    pos += 1;
}

void parse_primary(const std::vector<Token> &tokens, size_t &pos) {
    assert(false && "parse_primary is not implemented yet");
}

void parse_additive(const std::vector<Token> &tokens, size_t &pos) {
    parse_primary(tokens, pos);
    while (peek(tokens, pos).type == TokenType::PLUS ||
           peek(tokens, pos).type == TokenType::MINUS) {
        advance(tokens, pos);
        parse_primary(tokens, pos);
    }
}

void parse_expression(const std::vector<Token> &tokens, size_t &pos) {
    parse_additive(tokens, pos);
    while (peek(tokens, pos).type == TokenType::GREATER ||
           peek(tokens, pos).type == TokenType::LESS) {
        advance(tokens, pos);
        parse_additive(tokens, pos);
    }
}

void parse_statement(const std::vector<Token> &tokens, size_t &pos) {
    switch (peek(tokens, pos).type) {
    case TokenType::DEF:
        advance(tokens, pos);
        expect(tokens, pos, TokenType::IDENT);
        expect(tokens, pos, TokenType::LPAREN);
        while (peek(tokens, pos).type != TokenType::RPAREN &&
               peek(tokens, pos).type != TokenType::END) {
            parse_statement(tokens, pos);
        }
        expect(tokens, pos, TokenType::RPAREN);
        expect(tokens, pos, TokenType::COLON);
        parse_block(tokens, pos);
        break;
    case TokenType::IF:
        advance(tokens, pos);
        while (peek(tokens, pos).type != TokenType::COLON &&
               peek(tokens, pos).type != TokenType::END) {
            parse_expression(tokens, pos);
        }
        expect(tokens, pos, TokenType::COLON);
        parse_block(tokens, pos);
        break;
    default:
        advance(tokens, pos);
    }
}

void parse_block(const std::vector<Token> &tokens, size_t &pos) {
    expect(tokens, pos, TokenType::LBRACE);
    while (peek(tokens, pos).type != TokenType::RBRACE &&
           peek(tokens, pos).type != TokenType::END) {
        parse_statement(tokens, pos);
    }
    expect(tokens, pos, TokenType::RBRACE);
}
