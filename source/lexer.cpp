#include "lexer.hpp"

#include <cassert>
#include <cstddef>
#include <stack>
#include <string>
#include <vector>

std::vector<Line> mark_indents(const std::vector<Line> &lines) {

    std::vector<Line> lines_with_indent;
    std::stack<size_t> indent_stack;
    indent_stack.push(0);

    for (const auto &line : lines) {
        if (line.indent == indent_stack.top()) {

            lines_with_indent.push_back(line);
        } else if (line.indent > indent_stack.top()) {
            indent_stack.push(line.indent);
            lines_with_indent.push_back({
                "INDENT",
                line.indent,
                line.line_number,
            });
            lines_with_indent.push_back(line);
        } else {
            while (line.indent < indent_stack.top()) {
                indent_stack.pop();
                lines_with_indent.push_back({
                    "DEDENT",
                    line.indent,
                    line.line_number,
                });
            }
            if (line.indent != indent_stack.top()) {
                std::cerr << "IndentationError: " << line.text << "\n";
                throw std::runtime_error("Indentation Error");
            }
            lines_with_indent.push_back(line);
        }
    }

    while (indent_stack.top() != 0) {
        indent_stack.pop();
        lines_with_indent.push_back({"DEDENT", 0, lines[lines.size() - 1].line_number});
    }

    return lines_with_indent;
}

std::vector<Token> tokenize(const std::vector<Line> &lines) {
    std::vector<Token> tokens;

    for (const Line &line : lines) {
        size_t i = 0;
        std::string line_string = line.text;
        while (i < line_string.size()) {
            char c = line_string[i];

            if (std::isalpha(static_cast<unsigned char>(c)) || c == '_') {
                size_t start = i;
                while (i < line_string.size() &&
                       (std::isalnum(static_cast<unsigned char>(line_string[i])) ||
                        line_string[i] == '_')) {
                    i += 1;
                }
                std::string word = line_string.substr(start, i - start);

                if (word == "def") {
                    tokens.push_back({
                        word,
                        TokenType::DEF,
                        line.line_number,
                    });
                } else if (word == "if") {
                    tokens.push_back({
                        word,
                        TokenType::IF,
                        line.line_number,
                    });
                } else if (word == "INDENT") {
                    tokens.push_back({
                        "{",
                        TokenType::LBRACE,
                        line.line_number,
                    });
                } else if (word == "DEDENT") {
                    tokens.push_back({
                        "}",
                        TokenType::RBRACE,
                        line.line_number,
                    });
                } else {
                    tokens.push_back({
                        word,
                        TokenType::IDENT,
                        line.line_number,
                    });
                }
                continue;
            } else if (c == '"') {
                size_t start = i;
                i += 1;
                while (i < line_string.size() && line_string[i] != '"') {
                    i += 1;
                    if (i >= line_string.size()) {
                        std::cerr << "SyntaxError: unterminated string\n";
                        assert(false && "unterminated string");
                    }
                }
                tokens.push_back({
                    line_string.substr(start + 1, i - start - 1),
                    TokenType::STRING,
                    line.line_number,
                });
                i += 1;
                continue;
            } else if (std::isdigit(static_cast<unsigned char>(c))) {
                size_t start = i;
                while (i < line_string.size() &&
                       std::isdigit(static_cast<unsigned char>(line_string[i]))) {
                    i += 1;
                }
                tokens.push_back({
                    line_string.substr(start, i - start),
                    TokenType::NUMBER,
                    line.line_number,
                });
                continue;
            } else if (c == ' ') {
                i += 1;
                continue;
            } else if (c == '+') {
                i += 1;
                tokens.push_back({
                    "+",
                    TokenType::PLUS,
                    line.line_number,
                });
                continue;
            } else if (c == '-') {
                i += 1;
                tokens.push_back({
                    "-",
                    TokenType::MINUS,
                    line.line_number,
                });
                continue;
            } else if (c == '=') {
                i += 1;
                tokens.push_back({
                    "=",
                    TokenType::ASSIGN,
                    line.line_number,
                });
                continue;
            } else if (c == ':') {
                i += 1;
                tokens.push_back({
                    ":",
                    TokenType::COLON,
                    line.line_number,
                });
                continue;
            } else if (c == '(') {
                i += 1;
                tokens.push_back({
                    "(",
                    TokenType::LPAREN,
                    line.line_number,
                });
                continue;
            } else if (c == ')') {
                i += 1;
                tokens.push_back({
                    ")",
                    TokenType::RPAREN,
                    line.line_number,
                });
                continue;
            } else if (c == '>') {
                i += 1;
                tokens.push_back({
                    ">",
                    TokenType::GREATER,
                    line.line_number,
                });
            } else if (c == '<') {
                i += 1;
                tokens.push_back({
                    "<",
                    TokenType::LESS,
                    line.line_number,
                });
            } else {
                assert(false && "Unknown char expected");
            }
        }
    }
    tokens.push_back({
        "",
        TokenType::END,
        0,
    });
    return tokens;
}
