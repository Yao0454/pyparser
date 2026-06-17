#ifndef INTERPRETER_HPP_
#define INTERPRETER_HPP_

#include "parser.hpp"
#include <cstddef>

void itpt_funcdef(const Node &node);
void itpt_block(const Node &node);
void itpt_assign(const Node &node);
void iter_node(const Node &node);
void itpt_ident(const Node &node);
void itpt_number(const Node &node);
void itpt_if(const Node &node);
void itpt_binop(const Node &node);
void itpt_call(const Node &node);
void itpt_string(const Node &node);

#endif // INTERPRETER_HPP_
