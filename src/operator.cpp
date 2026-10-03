#include "operator.h"

#include <string>
#include <map>


std::map<Operator, std::string> operator_names = {
    {Operator::CMP_EQ, "="},
    {Operator::CMP_LT, "<"},
    {Operator::CMP_GT, ">"},
    {Operator::CMP_LEQ, "<="},
    {Operator::CMP_GEQ, ">="},
    {Operator::CMP_NEQ, "!="},
    {Operator::ARITH_ADD, "+"},
    {Operator::ARITH_SUB, "-"},
    {Operator::ARITH_MUL, "*"},
    {Operator::ARITH_DIV, "/"},
    {Operator::ARITH_MOD, "%"},
    {Operator::BIT_AND, "&"},
    {Operator::BIT_OR, "|"},
    {Operator::BIT_XOR, "^"},
    {Operator::BIT_NOT, "~"},
    {Operator::BIT_LSHIFT, "<<"},
    {Operator::BIT_RSHIFT, ">>"},
    {Operator::LOG_NOT, "!"},
    {Operator::LOG_AND, "&&"},
    {Operator::LOG_OR, "||"}
};

std::string op_to_string(Operator op)
{
    auto it = operator_names.find(op);
    if (it == operator_names.end())
        return "?";
    else return operator_names[op];
}