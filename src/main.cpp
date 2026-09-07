#include "message.h"
#include "token.h"
#include "lexer.h"

#include <iostream>
#include <string>

int main()
{
   // Message msg("NYA Hello World");
   // msg.print();

   std::string source(
        std::istreambuf_iterator<char>(std::cin),
        std::istreambuf_iterator<char>()
    );

    Lexer lexer;
    std::vector<Token> tokens = lexer.tokenize(source);

    for (Token t: tokens)
    {
        t.print();
    }
    return 0;
}
