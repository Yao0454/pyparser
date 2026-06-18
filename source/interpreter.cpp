#include "interpreter.hpp"
#include "parser.hpp"
#include <cassert>
#include <string>
#include <unordered_map>
#include <vector>

std::unordered_map<std::string, Value> variebles;

Value eval(const Node &node) {
    switch (node.type) {
    case NodeType::NUMBER:
        return Value{
            ValueType::NUMBER,
            stoi(node.value),
            "",
            false,
        };
    case NodeType::IDENT:
        if (node.value == "True") {
            return Value{
                ValueType::BOOLEAN,
                0,
                "",
                true,
            };
        } else if (node.value == "False") {
            return Value{
                ValueType::BOOLEAN,
                0,
                "",
                false,
            };
        } else {
            return variebles[node.value];
        }
    case NodeType::IF: {
        Node op = node.children[0];
        Node block = node.children[1];
    }
    default:
        assert(false && "[Error]: UNREABHABLE!");
    }
    assert(false && "function eval is not implemented yet");
}

void exec(const Node &node) {
    switch (node.type) {
    case NodeType::ASSIGN: {
        Node l = node.children[0];
        Node r = node.children[1];
        assert(l.type == NodeType::IDENT);
        if (r.type == NodeType::NUMBER) {
            variebles[l.value] = Value{
                ValueType::NUMBER,
                stoi(r.value),
                "",
                false,
            };
        } else if (r.type == NodeType::STRING) {
            variebles[l.value] = Value{
                ValueType::STRING,
                0,
                r.value,
                false,
            };
        } else if (r.type == NodeType::IDENT) {
            if (r.value == "True") {
                variebles[l.value] = Value{
                    ValueType::BOOLEAN,
                    0,
                    "",
                    true,
                };
            } else if (r.value == "False") {
                variebles[l.value] = Value{
                    ValueType::BOOLEAN,
                    0,
                    "",
                    false,
                };
            } else {
                variebles[l.value] = variebles[r.value];
            }
        }
    } break;
    default:
        assert(false && "function exec is not implemented yet");
    }
}

void itpt_funcdef(const Node &node) {
    for (const Node &child : node.children) {
        iter_node(child);
    }
}

void itpt_block(const Node &node) {
    for (const Node &child : node.children) {
        iter_node(child);
    }
}

void itpt_assign(const Node &node) { exec(node); }

void itpt_number(const Node &node) {
    (void)node;
    assert(false && "interprete number is not implemented yet!");
}

Value itpt_if(const Node &node) { return eval(node); }

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
