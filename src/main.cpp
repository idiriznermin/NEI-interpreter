#include "message.h"
#include "token.h"
#include "lexer.h"
#include "parser.h"
#include "ast.h"

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
    tokens.pop_back();
    Parser parser(tokens);
    std::unique_ptr<ExprNode> res = parser.parse_factor();
    if(res -> type == ExprType::INTEGER) std::cout << res -> integer << std::endl;

    return 0;
}
