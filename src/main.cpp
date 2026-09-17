#include "message.h"
#include "token.h"
#include "lexer.h"
#include "parser.h"
#include "ast.h"

#include <iostream>
#include <string>

void print_expr(const ExprNode* node, int depth = 0)
{
    if (!node)
        return;

    for (int i = 0; i < depth; ++i)
        std::cout << "  ";

    switch (node -> type)
    {
        case ExprType::INTEGER:
            std::cout << "INTEGER: " << node -> integer << '\n';
            break;

        case ExprType::VAR:
            std::cout << "VARIABLE: " << node -> var_name << '\n';
            break;

        case ExprType::BINARY:
            std::cout << "OPERATOR: " << node -> op << '\n';
            break;
    }

    print_expr(node->left.get(), depth + 1);
    print_expr(node->right.get(), depth + 1);
}

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
    //tokens.pop_back();
    Parser parser(tokens);
    std::unique_ptr<ExprNode> res = parser.parse_expression();
    print_expr(res.get());
    if(res -> type == ExprType::INTEGER) std::cout << res -> integer << std::endl;

    return 0;
}
