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

Node parse_primary(const std::vector<Token> &tokens, size_t &pos) {
    switch (peek(tokens, pos).type) {
    case TokenType::NUMBER: {
        Token current = advance(tokens, pos);
        return Node{
            NodeType::NUMBER,
            current.line_number,
            current.value,
            {},
        };
    }

    case TokenType::STRING: {
        Token current = advance(tokens, pos);
        return {
            NodeType::STRING,
            current.line_number,
            current.value,
            {},
        };
    }

    case TokenType::IDENT: {
        Token name = advance(tokens, pos);
        if (peek(tokens, pos).type == TokenType::LPAREN) {
            advance(tokens, pos);
            Node call = {
                NodeType::CALL,
                name.line_number,
                name.value,
                {},
            };
            while (peek(tokens, pos).type != TokenType::RPAREN &&
                   peek(tokens, pos).type != TokenType::END) {
                call.children.push_back(parse_expression(tokens, pos));
            }
            expect(tokens, pos, TokenType::RPAREN);
            return call;
        }
        return Node{
            NodeType::IDENT,
            name.line_number,
            name.value,
            {},
        };
    }
    case TokenType::LPAREN: {
        advance(tokens, pos);
        Node inner = parse_expression(tokens, pos);
        expect(tokens, pos, TokenType::RPAREN);
        return inner;
    }
    default:
        std::cerr << "[Error]: At Line " << peek(tokens, pos).line_number << " "
                  << peek(tokens, pos).value << "\n";
        throw std::runtime_error("Unexpeted token in expression\n");
    }
}

Node parse_additive(const std::vector<Token> &tokens, size_t &pos) {
    Node left = parse_primary(tokens, pos);
    while (peek(tokens, pos).type == TokenType::PLUS ||
           peek(tokens, pos).type == TokenType::MINUS) {
        Token op = advance(tokens, pos);
        Node right = parse_primary(tokens, pos);
        left = Node{
            NodeType::BINOP,
            op.line_number,
            op.value,
            {left, right},
        };
    }
    return left;
}

Node parse_expression(const std::vector<Token> &tokens, size_t &pos) {
    Node left = parse_additive(tokens, pos);
    while (peek(tokens, pos).type == TokenType::GREATER ||
           peek(tokens, pos).type == TokenType::LESS) {
        Token op = advance(tokens, pos);
        Node right = parse_additive(tokens, pos);
        left = Node{
            NodeType::BINOP,
            op.line_number,
            op.value,
            {left, right},
        };
    }
    return left;
}

Node parse_statement(const std::vector<Token> &tokens, size_t &pos) {
    switch (peek(tokens, pos).type) {
    case TokenType::DEF: {
        Token token = advance(tokens, pos);
        Token name = peek(tokens, pos);
        Node node = {
            NodeType::FUNCDEF,
            token.line_number,
            name.value,
            {},
        };
        expect(tokens, pos, TokenType::IDENT);
        expect(tokens, pos, TokenType::LPAREN);
        while (peek(tokens, pos).type != TokenType::RPAREN &&
               peek(tokens, pos).type != TokenType::END) {
            node.children.push_back(parse_statement(tokens, pos));
        }
        expect(tokens, pos, TokenType::RPAREN);
        expect(tokens, pos, TokenType::COLON);
        node.children.push_back(parse_block(tokens, pos));
        return node;
    }

    case TokenType::IF: {
        Token token = advance(tokens, pos);
        Node node = {
            NodeType::IF,
            token.line_number,
            "",
            {},
        };
        while (peek(tokens, pos).type != TokenType::COLON &&
               peek(tokens, pos).type != TokenType::END) {
            node.children.push_back(parse_expression(tokens, pos));
        }
        expect(tokens, pos, TokenType::COLON);
        node.children.push_back(parse_block(tokens, pos));

        if (peek(tokens, pos).type == TokenType::ELSE) {
            advance(tokens, pos);
            expect(tokens, pos, TokenType::COLON);
            node.children.push_back(parse_block(tokens, pos));
        }

        return node;
    }
    case TokenType::IDENT: {
        Token token = peek(tokens, pos);
        if (peek(tokens, pos + 1).type == TokenType::ASSIGN) {

            Node left = parse_expression(tokens, pos);
            expect(tokens, pos, TokenType::ASSIGN);
            Node node = {
                NodeType::ASSIGN,
                token.line_number,
                "",
                {left, parse_expression(tokens, pos)},
            };
            return node;
        } else {
            return parse_expression(tokens, pos);
        }
    }
    case TokenType::STRING: {
        Token token = advance(tokens, pos);
        return Node{
            NodeType::STRING,
            token.line_number,
            token.value,
            {},
        };
    }
    default:
        std::cerr << "[Error]: At Line " << peek(tokens, pos).line_number << " "
                  << peek(tokens, pos).value << "\n";
        throw std::runtime_error("Unexpected token at statement\n");
    }
}

Node parse_block(const std::vector<Token> &tokens, size_t &pos) {
    expect(tokens, pos, TokenType::LBRACE);
    Token token = peek(tokens, pos);
    Node node = {
        NodeType::BLOCK,
        token.line_number,
        "",
        {},
    };
    while (peek(tokens, pos).type != TokenType::RBRACE &&
           peek(tokens, pos).type != TokenType::END) {
        node.children.push_back(parse_statement(tokens, pos));
    }
    expect(tokens, pos, TokenType::RBRACE);
    return node;
}

const char *node_type_name(NodeType t) {
    switch (t) {
    case NodeType::NUMBER:
        return "NUMBER";
    case NodeType::IDENT:
        return "IDENT";
    case NodeType::BINOP:
        return "BINOP";
    case NodeType::STRING:
        return "STRING";
    case NodeType::ASSIGN:
        return "ASSIGN";
    case NodeType::CALL:
        return "CALL";
    case NodeType::BLOCK:
        return "BLOCK";
    case NodeType::IF:
        return "IF";
    case NodeType::FUNCDEF:
        return "FUNCDEF";
    }
    return "?";
}

void print_node(const Node &node, int depth) {
    for (int i = 0; i < depth; ++i)
        std::print("  ");

    std::println("{} {}", node_type_name(node.type), node.value);

    for (const Node &child : node.children) {
        print_node(child, depth + 1);
    }
}
