#include <cstddef>
#include <fstream>
#include <iostream>
#include <print>
#include <stack>
#include <string>
#include <vector>

struct Line {
    std::string line;
    size_t indent;
};

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

void parse_indent(const std::vector<Line> &lines) {

    std::stack<size_t> indent_stack;
    indent_stack.push(0);
    std::println("INDENT");

    for (auto line : lines) {
        if (line.indent == indent_stack.top()) {
            std::println("{}", line.line);
        } else if (line.indent > indent_stack.top()) {
            indent_stack.push(line.indent);
            std::println("INDENT");
            std::println("{}", line.line);
        } else {
            while (line.indent != indent_stack.top()) {
                indent_stack.pop();
                std::println("DEDENT");
            }
            std::println("{}", line.line);
        }
    }

    while (!indent_stack.empty()) {
        indent_stack.pop();
        std::println("DEDENT");
    }
}

int main() {
    std::ifstream file("./pyfile.py");
    if (!file) {
        std::cerr << "Unable to open the file!\n";
        return 1;
    }

    std::vector<Line> lines;
    std::string buf;
    while (std::getline(file, buf)) {
        if (buf.empty())
            continue;
        lines.push_back({strip(buf), count_indent(buf)});
    }
    file.close();

    parse_indent(lines);
}
