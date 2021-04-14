#include <cctype>

#include <stdexcept>

#include "lexis.hxx"

std::vector<Token> tokenize(std::string_view expression) {
    std::vector<Token> tokens;
    while (!expression.empty()) {
        if (!std::isgraph(static_cast<unsigned char>(expression.front()))) {
            expression.remove_prefix(1);
            continue;
        }
        switch (expression.front()) {
            case '(':
            case ')':
                tokens.push_back(Punctuator{expression.front()});
                expression.remove_prefix(1);
                break;
            case '"': {
                expression.remove_prefix(1);
                std::string content;
                {   bool closed = false;
                    while (!expression.empty()) {
                        if (expression.front() == '"') {
                            closed = true;
                            expression.remove_prefix(1);
                            break;
                        } else {
                            content += expression.front();
                            expression.remove_prefix(1);
                        }
                    }
                    if (!closed)
                        throw std::invalid_argument("Non-terminated string literal.");
                }
                tokens.push_back(StringLiteral{content});
                break;
            }
            default: {
                if (!std::isalnum(static_cast<unsigned char>(expression.front())) && expression.front() != '-')
                    throw std::invalid_argument("Invalid character.");
                std::string word;
                word += expression.front();
                expression.remove_prefix(1);
                while (!expression.empty()) {
                    if (std::isalnum(static_cast<unsigned char>(expression.front())) || expression.front() == '-') {
                        word += expression.front();
                        expression.remove_prefix(1);
                    } else {
                        break;
                    }
                }
                tokens.push_back(Word{word});
                break;
            }
        }
    }
    return tokens;
}
