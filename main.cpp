#include "lexer.hpp"
#include "parser.hpp"
#include <cstddef>
#include <fstream>
#include <print>
#include <stdexcept>
#include <string>
#include <vector>

size_t count_indent(const std::string &line) {
    size_t n = 0;
    for (char c : line) {
        if (c == ' ') {
            n += 1;
        } else if (c == '\t') {
            n += 8;
        } else
            break;
    }
    return n;
}

std::string strip(const std::string &s) {
    const char *ws = " \t\n\r\f\v";
    size_t start = s.find_first_not_of(ws);
    if (start == std::string::npos)
        return "";
    size_t end = s.find_last_not_of(ws);
    return s.substr(start, end - start + 1);
}

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cerr << "[Error]: please provide the .py file!\nUsage pyparse <file_path>\n";
        exit(1);
    }
    std::string file_path = argv[1];
    std::ifstream file(file_path);
    if (!file) {
        std::cerr << "Unable to open the file!\n";
        return 1;
    }

    std::vector<Line> lines;
    std::string buf;
    size_t current_line_number = 1;
    while (std::getline(file, buf)) {
        if (strip(buf).empty())
            continue;
        lines.push_back({strip(buf), count_indent(buf), current_line_number});
        current_line_number += 1;
    }
    file.close();

    std::vector<Line> lines_with_indent = mark_indents(lines);

    std::vector<Token> tokens = tokenize(lines_with_indent);

    // for (const Token &token : tokens) {
    //     std::println("{}", token.value);
    // }

    size_t pos = 0;
    while (peek(tokens, pos).type != TokenType::END) {
        parse_statement(tokens, pos);
    }
    std::println("Parse OK");
}
