#include "operator.h"

#include <cassert>

const std::map<Operator, std::string> OPERATOR_TO_STRING = {
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

std::string operator_to_string(Operator op)
{
    auto it = OPERATOR_TO_STRING.find(op);
    assert(it != OPERATOR_TO_STRING.end());
    return it->second;
}