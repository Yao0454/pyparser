#ifndef INTERPRETER_HPP_
#define INTERPRETER_HPP_

#include "parser.hpp"
#include <cstddef>
#include <string>

enum class ValueType {
    NUMBER,
    STRING,
    BOOLEAN,
};

struct Value {
    ValueType type;
    int number = 0;
    std::string str;
    bool boolean = false;
};

Value eval(const Node &node);
void exec(const Node &node);

void iter_node(const Node &node);
void itpt_funcdef(const Node &node);
void itpt_block(const Node &node);
void itpt_assign(const Node &node);
void itpt_number(const Node &node);
Value itpt_if(const Node &node);
void itpt_binop(const Node &node);
void itpt_call(const Node &node);
void itpt_string(const Node &node);

#endif // INTERPRETER_HPP_
