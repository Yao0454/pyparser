#ifndef INTERPRETER_HPP_
#define INTERPRETER_HPP_

#include "parser.hpp"
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

#endif
