#include <cstdlib>
#include <cassert>

#include <string_view>
#include <string>
#include <iostream>

#include "lexis.hxx"
#include "syntax.hxx"
#include "evaluation.hxx"

using namespace std::literals;

int main() {
    while (true) {
        std::cout << "$ "sv;
        std::string expression;
        if (!getline(std::cin, expression)) {
            std::cerr << "Failed to scan.\n"sv;
            return EXIT_FAILURE;
        }
        const std::vector<Token> tokens = tokenize(expression);
        if (tokens.size() == 1) {
            if (const auto * const word = std::get_if<Word>(&tokens.front())) {
                if (*word == "quit"sv)
                    break;
            }
        }
        const std::unique_ptr<Node> node = parse(tokens);
        assert(node);
        std::cout << "> "sv << evaluate(*node) << '\n';
    }
    return EXIT_SUCCESS;
}
