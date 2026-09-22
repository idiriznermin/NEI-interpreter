#include "token.h"
#include "parser.h"
#include "ast.h"

#include <vector>

std::unique_ptr<ExprNode> Parser::parse_expression()
{
    return parse_logical();
}

std::unique_ptr<ExprNode> Parser::parse_logical()
{
    std::unique_ptr<ExprNode> left = Parser::parse_comparison();
    while (Parser::peek().type == Token::Type::OP_LOGICAL)
    {
        Token curr_op = Parser::advance();
        std::unique_ptr<ExprNode> right = Parser::parse_comparison();

        auto node = std::make_unique<ExprNode>();
        node -> type = ExprType::BINARY;
        node -> left = std::move(left);
        node -> right = std::move(right);
        node -> op = std::get<std::string>(curr_op.value);
        left = std::move(node);
    }
    return left;
}

std::unique_ptr<ExprNode> clone_expr(const ExprNode* node)
{
    if (!node)
        return nullptr;

    auto copy = std::make_unique<ExprNode>();
    copy -> type = node -> type;
    copy -> integer = node -> integer;
    copy -> var_name = node -> var_name;
    copy -> op = node -> op;
    copy -> left = clone_expr(node -> left.get());
    copy -> right = clone_expr(node -> right.get());
    copy -> unary_op = node -> unary_op;
    copy -> unary_child = clone_expr(node -> unary_child.get());

    return copy;
}

std::unique_ptr<ExprNode> Parser::parse_comparison()
{
    std::unique_ptr<ExprNode> first = Parser::parse_arith();

    std::vector<std::unique_ptr<ExprNode>> operands;
    std::vector<std::string> operations;

    operands.push_back(std::move(first));

    while (Parser::peek().type == Token::Type::OP_COMPARE)
    {
        Token curr_op = Parser::advance();
        operations.push_back(std::get<std::string>(curr_op.value));
        operands.push_back(Parser::parse_arith());
    }

    std::unique_ptr<ExprNode> result = nullptr;
    if (operations.empty())
    {
        result = std::move(operands[0]);
        return result;
    }

    int n = (int)(operations.size());
    for (int i = 0; i < n; ++ i)
    {
        auto cmp = std::make_unique<ExprNode>();
        cmp -> type = ExprType::BINARY;
        cmp -> op = operations[i];
        cmp -> left = std::move(operands[i]);

        if (i == n - 1) cmp -> right = std::move(operands[i+1]);
        else
        {
            cmp -> right = clone_expr(operands[i+1].get());
        }

        if(result == nullptr)
        {
            result = std::move(cmp);
        }
        else
        {
            auto and_node = std::make_unique<ExprNode>();
            and_node -> type = ExprType::BINARY;
            and_node -> op = "&&";
            and_node -> left = std::move(result);
            and_node -> right = std::move(cmp);
            result = std::move(and_node);
        }
    }

    return result;
}

std::unique_ptr<ExprNode> Parser::parse_arith()
{
    std::unique_ptr<ExprNode> left = Parser::parse_term();

    while (Parser::peek().type == Token::Type::OP_ADD)
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
    std::unique_ptr<ExprNode> left = Parser::parse_unary();
    while (Parser::peek().type == Token::Type::OP_MUL)
    {
        Token curr_op = Parser::advance();
        std::unique_ptr<ExprNode> right = Parser::parse_unary();

        auto node = std::make_unique<ExprNode>();
        node -> type = ExprType::BINARY;
        node -> left = std::move(left);
        node -> right = std::move(right);
        node -> op = std::get<std::string>(curr_op.value);
        left = std::move(node);
    }
    return left;
}

std::unique_ptr<ExprNode> Parser::parse_unary()
{
    Token curr_token = Parser::peek();

    if (curr_token.type == Token::Type::OP_NOT ||
        (curr_token.type == Token::Type::OP_ADD && std::get<std::string>(curr_token.value) == "-") ||
        (curr_token.type == Token::Type::OP_BITWISE && std::get<std::string>(curr_token.value) == "~"))
    {
        Parser::advance();

        auto node = std::make_unique<ExprNode>();
        node -> type = ExprType::UNARY;
        if(curr_token.type == Token::Type::OP_NOT)node -> unary_op = "!";
        else if(curr_token.type == Token::Type::OP_ADD)node -> unary_op = "-";
        else node -> unary_op = "~";
        node -> unary_child = Parser::parse_unary();

        return node;
    }
    return Parser::parse_factor();
}

std::unique_ptr<ExprNode> Parser::parse_factor()
{
    Token curr_token = Parser::peek();

    auto ans = std::make_unique<ExprNode>();
    if (curr_token.type == Token::Type::LITERAL_INTEGER)
    {
        ans -> type = ExprType::INTEGER;
        ans -> integer = std::get<int>(curr_token.value);
        Parser::advance();
        return ans;
    }
    if (curr_token.type == Token::Type::IDENTIFIER)
    {
        ans->type = ExprType::VAR;
        ans->var_name = std::get<std::string>(curr_token.value);
        Parser::advance();
        return ans;
    }
    if (curr_token.type == Token::Type::LEFT_BRACKET)
    {
        Parser::advance();
        ans = Parser::parse_expression();

        Parser::expect(Token::Type::RIGHT_BRACKET, "expected ')' ");
        return ans;
    }
    std::cout << "Unexpected token at line " << curr_token.line << std::endl;
    exit(1);
}

