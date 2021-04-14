#include <cassert>

#include <string_view>
#include <charconv>
#include <stdexcept>
#include <map>
#include <algorithm>

#include "evaluation.hxx"

using namespace std::literals;

static int to_number(const std::string_view word) {
    int number;
    const std::from_chars_result fcr = std::from_chars(word.data(), word.data() + word.size(), number);
    if (fcr.ec == std::errc::result_out_of_range)
        throw std::range_error("Number too large.");
    if (fcr.ec == std::errc::invalid_argument)
        throw std::invalid_argument("Not a number.");
    assert(fcr.ec == std::errc{});
    if (fcr.ptr != word.data() + word.size())
        throw std::invalid_argument("Number not completely matched.");
    return number;
}

using Function = Value(const std::vector<Value> &args);

static const std::map<std::string_view, Function *> functions = {
    {"add"sv, [](const std::vector<Value> &args) {
        int n = 0;
        for (const Value &a: args) {
            n += to_number(a);
        }
        return Value{std::to_string(n)};
    }},
    {"sub"sv, [](const std::vector<Value> &args) {
        if (args.size() < 2)
            throw std::invalid_argument("Not enough arguments for sub function.");
        if (args.size() > 2)
            throw std::invalid_argument("Too many arguments for sub function.");
        assert(args.size() == 2);
        const int number0 = to_number(args[0]);
        const int number1 = to_number(args[1]);
        return Value{std::to_string(number0 - number1)};
    }},
    {"mult"sv, [](const std::vector<Value> &args) {
        int n = 1;
        for (const Value &a: args) {
            n *= to_number(a);
        }
        return Value{std::to_string(n)};
    }},
    {"concat"sv, [](const std::vector<Value> &args) {
        std::string r;
        for (const Value &a: args) {
            r += a;
        }
        return Value{r};
    }},
};

static Value evaluate(const AtomNode &node) {
    return Value{node.value};
}

static Value evaluate(const FunctionCallNode &node) {
    Function * const function = [&]{
        const auto i = functions.find(node.function_name);
        if (i == functions.cend())
            throw std::invalid_argument("Unknown function.");
        return i->second;
    }();
    std::vector<Value> args(node.arguments.size());
    std::transform(node.arguments.cbegin(), node.arguments.cend(),
                   args.begin(),
                   [](const std::unique_ptr<Node> &node){
                       assert(node);
                       return evaluate(*node);
                   });
    return function(args);
}

Value evaluate(const Node &node) {
    if (const AtomNode * const atom_node = dynamic_cast<const AtomNode *>(&node)) {
        return evaluate(*static_cast<const AtomNode *>(atom_node));
    }
    if (const FunctionCallNode * const function_call_node = dynamic_cast<const FunctionCallNode *>(&node)) {
        return evaluate(*static_cast<const FunctionCallNode *>(function_call_node));
    }
    throw std::invalid_argument("Invalid kind of node.");
}
