#pragma once

#include "token.h"
#include "token_stream.h"

#include <string>
#include <vector>

class Lexer
{

public:
    Lexer() = default;
    TokenStream tokenize(const std::string& source);

};
