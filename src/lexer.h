#pragma once

#include "token.h"

#include <string>
#include <vector>

class Lexer
{

public:
    Lexer();
    std::vector<Token> tokenize(const std::string& source);

};
