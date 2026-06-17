#ifndef PARSER_HPP_
#define PARSER_HPP_

#include "lexer.hpp"
#include <cstddef>
#include <vector>

enum class NodeType {
    NUMBER,
    IDENT,
    STRING,
    BINOP,
    ASSIGN,
    CALL,
    BLOCK,
    IF,
    FUNCDEF,
};

struct Node {
    NodeType type;
    size_t line_number;
    std::string value;
    std::vector<Node> children;
};

Token peek(const std::vector<Token> &tokens, size_t pos);
Token advance(const std::vector<Token> &tokens, size_t &pos);
void expect(const std::vector<Token> &tokens, size_t &pos, TokenType t);

Node parse_primary(const std::vector<Token> &tokens, size_t &pos);
Node parse_additive(const std::vector<Token> &tokens, size_t &pos);
Node parse_expression(const std::vector<Token> &tokens, size_t &pos);
Node parse_statement(const std::vector<Token> &tokens, size_t &pos);
Node parse_block(const std::vector<Token> &tokens, size_t &pos);

const char *node_type_name(NodeType t);
void print_node(const Node &node, int depth);

#endif // PARSER_HPP_
