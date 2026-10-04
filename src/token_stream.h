#pragma once

#include "token.h"

#include <vector>

class TokenStream
{
public:
    std::vector<Token> tokens;
    TokenStream(std::vector<Token> tokens):
        tokens(tokens)
    {}
};
