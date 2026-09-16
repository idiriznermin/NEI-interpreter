#include "token.h"
#include "parser.h"
#include "ast.h"

#include <vector>
std::unique_ptr<ExprNode> Parser::parse_expression()
{
    return parse_factor();
}

std::unique_ptr<ExprNode> Parser::parse_factor()
{
    Token curr_token = Parser::peek();

    auto ans = std::make_unique<ExprNode>();
    if(curr_token.type == Token::Type::LITERAL_INTEGER)
    {
        ans->type = ExprType::INTEGER;
        ans->integer = std::get<int>(curr_token.value);
        Parser::advance();
        return ans;
    }
    if(curr_token.type == Token::Type::IDENTIFIER)
    {
        ans->type = ExprType::VAR;
        ans->var_name = std::get<std::string>(curr_token.value);
        Parser::advance();
        return ans;
    }
    if(curr_token.type == Token::Type::LEFT_BRACKET)
    {
        Parser::advance();
        ans = Parser::parse_expression();

        Parser::expect(Token::Type::RIGHT_BRACKET, "expected ')' ");
        return ans;
    }
    std::cout << "Unexpected token at line " << curr_token.line << std::endl;
    exit(1);
}

