#include "parser.hpp"

#include <stack>
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
        std::println("DEDENT");
    }
}
