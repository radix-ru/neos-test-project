#ifndef LEXIS_HXX
#define LEXIS_HXX

#include <string>
#include <variant>
#include <vector>

struct Word: std::string {
};

struct StringLiteral {
    std::string content;
};

enum class Punctuator: char;

using Token = std::variant<Word, StringLiteral, Punctuator>;

std::vector<Token> tokenize(std::string_view expression);

#endif // LEXIS_HXX
