#include "interpreter.hpp"
#include "parser.hpp"
#include <cassert>

void itpt_funcdef(const Node &node) {
    (void)node;
    assert(false && "interprete function def is not implemented yet!");
}

void itpt_block(const Node &node) {
    (void)node;
    assert(false && "interprete code is not implemented yet!");
}

void itpt_assign(const Node &node) {
    (void)node;
    assert(false && "interprete assign is not implemented yet!");
}

void itpt_ident(const Node &node) {
    (void)node;
    assert(false && "interprete ident is not implemented yet!");
}

void itpt_number(const Node &node) {
    (void)node;
    assert(false && "interprete number is not implemented yet!");
}

void itpt_if(const Node &node) {
    (void)node;
    assert(false && "interprete if is not implemented yet!");
}

void itpt_binop(const Node &node) {
    (void)node;
    assert(false && "interprete binop is not implemented yet!");
}

void itpt_call(const Node &node) {
    (void)node;
    assert(false && "interprete function call is not implemented yet!");
}

void itpt_string(const Node &node) {
    (void)node;
    assert(false && "interprete string is not implemented yet!");
}

void iter_node(const Node &node) {
    switch (node.type) {
    case NodeType::FUNCDEF:
        itpt_funcdef(node);
        break;
    case NodeType::BLOCK:
        itpt_block(node);
        break;
    case NodeType::ASSIGN:
        itpt_assign(node);
        break;
    case NodeType::IDENT:
        itpt_ident(node);
        break;
    case NodeType::NUMBER:
        itpt_number(node);
        break;
    case NodeType::IF:
        itpt_if(node);
        break;
    case NodeType::BINOP:
        itpt_binop(node);
        break;
    case NodeType::CALL:
        itpt_call(node);
        break;
    case NodeType::STRING:
        itpt_string(node);
        break;
    default:
        assert(false && "iter_node: UNREACHABLE!");
    }
}
