#include "token.h"
#include "parser.h"
#include "ast.h"

#include <vector>
std::unique_ptr<ExprNode> Parser::parse_expression()
{
    return parse_arith();
}

std::unique_ptr<ExprNode> Parser::parse_arith()
{
    std::unique_ptr<ExprNode> left = Parser::parse_term();
    while(Parser::peek().type == Token::Type::OP_ADD)
    {
        Token curr_op = Parser::advance();
        std::unique_ptr<ExprNode> right = Parser::parse_term();

        auto node = std::make_unique<ExprNode>();
        node -> type = ExprType::BINARY;
        node -> left = std::move(left);
        node -> right = std::move(right);
        node -> op = std::get<std::string>(curr_op.value);
        left = std::move(node);
    }
    return left;
}
std::unique_ptr<ExprNode> Parser::parse_term()
{
    std::unique_ptr<ExprNode> left = Parser::parse_factor();
    while(Parser::peek().type == Token::Type::OP_MUL)
    {
        Token curr_op = Parser::advance();
        std::unique_ptr<ExprNode> right = Parser::parse_factor();

        auto node = std::make_unique<ExprNode>();
        node -> type = ExprType::BINARY;
        node -> left = std::move(left);
        node -> right = std::move(right);
        node -> op = std::get<std::string>(curr_op.value);
        left = std::move(node);
    }
    return left;
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

