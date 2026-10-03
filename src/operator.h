#pragma once

#include<string>

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

std::string op_to_string(Operator op);