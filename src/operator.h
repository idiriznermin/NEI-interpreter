#pragma once

#include <string>
#include <map>

enum class Operator
{
    CMP_EQ,
    CMP_LT,
    CMP_GT,
    CMP_LEQ,
    CMP_GEQ,
    CMP_NEQ,

    ARITH_ADD,
    ARITH_SUB,
    ARITH_MUL,
    ARITH_DIV,
    ARITH_MOD,

    BIT_AND,
    BIT_OR,
    BIT_XOR,
    BIT_NOT,
    BIT_LSHIFT,
    BIT_RSHIFT,

    LOG_NOT,
    LOG_AND,
    LOG_OR
};

extern const std::map<Operator, std::string> OPERATOR_TO_STRING;
std::string operator_to_string(Operator op);