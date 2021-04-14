#ifndef SYNTAX_HXX
#define SYNTAX_HXX

#include <string>
#include <vector>
#include <memory>

#include "lexis.hxx"

class Node {
public:
    virtual ~Node();
};

class AtomNode: public Node {
public:
    std::string value;
};

class FunctionCallNode: public Node {
public:
    std::string function_name;
    std::vector<std::unique_ptr<Node>> arguments;
};

std::unique_ptr<Node> parse(const std::vector<Token> &tokens);

#endif // SYNTAX_HXX
