#include "parser.hpp"

#include <cassert>
#include <stack>
#include <string>
#include <vector>

std::vector<std::string> parse_indent(const std::vector<Line> &lines) {

    std::vector<std::string> lines_with_indent;
    std::stack<size_t> indent_stack;
    indent_stack.push(0);

    for (const auto &line : lines) {
        if (line.indent == indent_stack.top()) {

            lines_with_indent.push_back(line.line);
        } else if (line.indent > indent_stack.top()) {
            indent_stack.push(line.indent);
            lines_with_indent.push_back("INDENT");
            lines_with_indent.push_back(line.line);
        } else {
            while (line.indent < indent_stack.top()) {
                indent_stack.pop();
                lines_with_indent.push_back("DEDENT");
            }
            if (line.indent != indent_stack.top()) {
                std::cerr << "IndentationError: " << line.line << "\n";
                throw std::runtime_error("Indentation Error");
            }
            lines_with_indent.push_back(line.line);
        }
    }

    while (indent_stack.top() != 0) {
        indent_stack.pop();
        lines_with_indent.push_back("DEDENT");
    }

    return lines_with_indent;
}

std::vector<Token> parse_token(const std::vector<std::string> &lines) {
    std::vector<Token> tokens;

    for (const std::string &line : lines) {
        size_t i = 0;
        while (i < line.size()) {
            char c = line[i];

            if (std::isalpha(static_cast<unsigned char>(c)) || c == '_') {
                size_t start = i;
                while (i < line.size() &&
                       (std::isalnum(static_cast<unsigned char>(line[i])) || line[i] == '_')) {
                    i += 1;
                }
                std::string word = line.substr(start, i - start);

                if (word == "def") {
                    tokens.push_back({word, TokenType::DEF});
                } else if (word == "if") {
                    tokens.push_back({word, TokenType::IF});
                } else if (word == "INDENT") {
                    tokens.push_back({"{", TokenType::LCBRAK});
                } else if (word == "DEDENT") {
                    tokens.push_back({"}", TokenType::RCBRAK});
                } else {
                    tokens.push_back({word, TokenType::IDENT});
                }
                continue;
            } else if (std::isdigit(static_cast<unsigned char>(c))) {
                size_t start = i;
                while (i < line.size() && std::isdigit(static_cast<unsigned char>(line[i]))) {
                    i += 1;
                }
                tokens.push_back({line.substr(start, i - start), TokenType::NUMBER});
                continue;
            } else if (c == ' ') {
                i += 1;
                continue;
            } else if (c == '+') {
                i += 1;
                tokens.push_back({"+", TokenType::ADD});
                continue;
            } else if (c == '=') {
                i += 1;
                tokens.push_back({"=", TokenType::ASSIGN});
                continue;
            } else if (c == ':') {
                i += 1;
                tokens.push_back({":", TokenType::COLOM});
                continue;
            } else if (c == '(') {
                i += 1;
                tokens.push_back({"(", TokenType::LBRAK});
                continue;
            } else if (c == ')') {
                i += 1;
                tokens.push_back({")", TokenType::RBRAK});
                continue;
            } else if (c == '>') {
                i += 1;
                tokens.push_back({">", TokenType::GREATER});
            } else {
                assert(false && "Unknown char expected");
            }
        }
    }
    return tokens;
}
