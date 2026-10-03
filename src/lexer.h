#pragma once

#include "token.h"

#include <string>
#include <vector>

class Lexer
{

public:
    Lexer() = default;
    std::vector<Token> tokenize(const std::string& source);

};
