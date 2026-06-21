#include "interpreter.hpp"
#include "parser.hpp"
#include <cstdlib>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

std::unordered_map<std::string, Value> variables;
std::unordered_map<std::string, Node> functions;

Value eval(const Node &node) {
    switch (node.type) {
    case NodeType::NUMBER:
        return Value{
            ValueType::NUMBER,
            std::stoi(node.value),
            "",
            false,
        };

    case NodeType::STRING:
        return Value{
            ValueType::STRING,
            0,
            node.value,
            false,
        };

    case NodeType::IDENT:
        if (node.value == "True")
            return Value{
                ValueType::BOOLEAN,
                0,
                "",
                true,
            };
        if (node.value == "False")
            return Value{
                ValueType::BOOLEAN,
                0,
                "",
                false,
            };
        return variables[node.value];

    case NodeType::BINOP: {
        Value l = eval(node.children[0]);
        Value r = eval(node.children[1]);
        if (l.type != ValueType::NUMBER || r.type != ValueType::NUMBER) {
            std::cerr << "[Error]: binop needs number operands\n";
            exit(1);
        }
        if (node.value == "+")
            return Value{
                ValueType::NUMBER,
                l.number + r.number,
                "",
                false,
            };
        if (node.value == "-")
            return Value{
                ValueType::NUMBER,
                l.number - r.number,
                "",
                false,
            };
        if (node.value == ">")
            return Value{
                ValueType::BOOLEAN,
                0,
                "",
                l.number > r.number,
            };
        if (node.value == "<")
            return Value{
                ValueType::BOOLEAN,
                0,
                "",
                l.number < r.number,
            };
        std::cerr << "[Error]: unknown operator " << node.value << "\n";
        exit(1);
    }

    default:
        std::cerr << "[Error]: eval cannot handle this node\n";
        exit(1);
    }
}

static void builtin_print(const Node &call) {
    for (size_t i = 0; i < call.children.size(); ++i) {
        if (i > 0)
            std::cout << " ";
        Value v = eval(call.children[i]);
        switch (v.type) {
        case ValueType::NUMBER:
            std::cout << v.number;
            break;
        case ValueType::STRING:
            std::cout << v.str;
            break;
        case ValueType::BOOLEAN:
            std::cout << (v.boolean ? "True" : "False");
            break;
        }
    }
    std::cout << "\n";
}

void exec(const Node &node) {
    switch (node.type) {
    case NodeType::FUNCDEF:
        functions[node.value] = node;
        break;

    case NodeType::BLOCK:
        for (const Node &child : node.children)
            exec(child);
        break;

    case NodeType::ASSIGN:
        variables[node.children[0].value] = eval(node.children[1]);
        break;

    case NodeType::IF:
        if (eval(node.children[0]).boolean) {
            exec(node.children[1]);
        } else if (node.children.size() > 2) {
            exec(node.children[2]);
        }
        break;

    case NodeType::CALL:
        if (node.value == "print") {
            builtin_print(node);
        } else if (functions.count(node.value)) {
            const Node &fn = functions[node.value];
            exec(fn.children.back());
        } else {
            std::cerr << "[Error]: unknown function " << node.value << "\n";
            exit(1);
        }
        break;

    default:
        std::cerr << "[Error]: exec cannot handle this statement\n";
        exit(1);
    }
}
