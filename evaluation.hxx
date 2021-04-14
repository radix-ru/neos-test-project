#ifndef EVALUATION_HXX
#define EVALUATION_HXX

#include <string>

#include "syntax.hxx"

struct Value: std::string {
};

Value evaluate(const Node &node);

#endif // EVALUATION_HXX
