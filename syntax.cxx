#include <cassert>

#include <stdexcept>

#include "syntax.hxx"

Node::~Node() = default;

/// @pre first
/// @pre last
static std::unique_ptr<Node> parse(const Token * &first, const Token * const last) {
    assert(first);
    assert(last);
    if (first == last)
        throw std::invalid_argument("No tokens to parse.");
    const Word * const function_name = std::get_if<Word>(first);
    if (!function_name)
        throw std::invalid_argument("Function call starts with not-a-word.");
    std::unique_ptr<FunctionCallNode> fcn = std::make_unique<FunctionCallNode>();
    fcn->function_name = *function_name;
    first++;
    while (first != last) {
        if (const Punctuator * const punctuator = std::get_if<Punctuator>(first)) {
            if (*punctuator == Punctuator{')'})
                break;
            if (*punctuator != Punctuator{'('})
                throw std::invalid_argument("Unexpected punctuator.");
            first++;
            fcn->arguments.push_back(parse(first, last));
            if (first == last)
                throw std::invalid_argument("Unterminated function call.");
            if (const Punctuator * const punctuator2 = std::get_if<Punctuator>(first)) {
                if (*punctuator2 != Punctuator{')'})
                    throw std::invalid_argument("')' not found.");
            } else
                throw std::invalid_argument("')' not found.");
            first++;
        } else if (const Word * const word = std::get_if<Word>(first)) {
            std::unique_ptr<AtomNode> node = std::make_unique<AtomNode>();
            node->value = *word;
            fcn->arguments.push_back(std::move(node));
            first++;
        } else if (const StringLiteral * const literal = std::get_if<StringLiteral>(first)) {
            std::unique_ptr<AtomNode> node = std::make_unique<AtomNode>();
            node->value = literal->content;
            fcn->arguments.push_back(std::move(node));
            first++;
        } else {
            assert(false);
        }
    }
    return fcn;
}

std::unique_ptr<Node> parse(const std::vector<Token> &tokens) {
    const Token *first = tokens.data();
    return parse(first, first + tokens.size());
}
