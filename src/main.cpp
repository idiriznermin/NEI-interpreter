#include "message.h"
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
    Lexer lexer(source);
    std::vector <Token> tokens = lexer.tokenize();
    return 0;
}
