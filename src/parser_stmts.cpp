#include "token.h"
#include "parser.h"
#include "ast.h"

#include <vector>

std::unique_ptr<StmtNode> Parser::parse_assignment()
{
    auto node = std::make_unique<StmtNode>();
    node -> type = StmtType::ASSIGN;

    Token id = Parser::expect(Token::Type::IDENTIFIER, "expected identifier");
    node -> assign_targets.push_back(std::get<std::string>(id.value));

    while(Parser::peek().type == Token::Type::COMMA)
    {
        Parser::advance();
        id = Parser::expect(Token::Type::IDENTIFIER, "expected identifier");
        node -> assign_targets.push_back(std::get<std::string>(id.value));
    }

    Parser::expect(Token::Type::ASSIGN, "expected ':='");

    std::unique_ptr<ExprNode> val = Parser::parse_expression();
    node -> assign_values.push_back(std::move(val));

    while(Parser::peek().type == Token::Type::COMMA)
    {
        Parser::advance();
        val = Parser::parse_expression();
        node -> assign_values.push_back(std::move(val));
    }
    return node;
}

std::unique_ptr<StmtNode> Parser::parse_declaration()
{
    auto node = std::make_unique<StmtNode>();

    node -> type = StmtType::DECL;

    Token curr_token = Parser::advance();
    if(curr_token.type == Token::Type::CONST)
    {
        node -> is_const = 1;
    }
    else
    {
        node -> is_const = 0;
        if(curr_token.type != Token::Type::VAR){
             std::cout << "Unexpected token at line" << curr_token.line << std::endl;
             exit(1);
        }
    }

    Token type_token = Parser::advance();
    if(type_token.type == Token::Type::TYPE_BOOL)node -> decl_type =  "bool";
    else if(type_token.type == Token::Type::TYPE_CHAR)node -> decl_type = "char";
    else if(type_token.type == Token::Type::TYPE_INT)node -> decl_type = "int";
    else {
        std::cout << "Unexpected token at line" << type_token.line << std::endl;
        exit(1);
    }

    Parser::expect(Token::Type::COLON, "expected ':' after type");

    while(Parser::peek().type == Token::Type::IDENTIFIER)
    {
        Token id = Parser::advance();
        node -> decl_names.push_back(std::get<std::string>(id.value));

        if(Parser::peek().type == Token::Type::ASSIGN)
        {
            Parser::advance();
            node -> decl_init.push_back(Parser::parse_expression());
        }
        else node -> decl_init.push_back(nullptr);
        if(Parser::peek().type == Token::Type::COMMA)Parser::advance();
        else break;
    }
    return node;
}