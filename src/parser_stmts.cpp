#include "token.h"
#include "parser.h"
#include "ast.h"

#include <vector>

std::vector<std::unique_ptr<StmtNode>> Parser::parse_program()
{
    std::vector<std::unique_ptr<StmtNode>> node;
    node = parse_statement_list();
    expect(Token::Type::EOFILE, "Error: expected eofile token");
    return node;
}

std::unique_ptr<StmtNode> Parser::parse_while_statement()
{
    auto node = std::make_unique<StmtNode>();
    node->type = StmtType::WHILE;

    expect(Token::Type::WHILE, "Error: expected while token");

    expect(Token::Type::LEFT_BRACKET, "Error: expected left bracket");
    node->while_cond = std::move(parse_expression());
    expect(Token::Type::RIGHT_BRACKET, "Error: expected right bracket");

    node->while_body = parse_statement_list();
    while (peek().type == Token::Type::EOLINE)
        advance();
    expect(Token::Type::ENDWHILE, "Error: expected endwhile token");
    return node;
}

std::unique_ptr<StmtNode> Parser::parse_if_statement()
{
    auto node = std::make_unique<StmtNode>();
    node->type = StmtType::IF;

    expect(Token::Type::IF, "Error: expected if token");

    expect(Token::Type::LEFT_BRACKET, "Error: expected left bracket");
    node->if_cond = std::move(parse_expression());
    expect(Token::Type::RIGHT_BRACKET, "Error: expected right bracket");

    node->if_then_body = parse_statement_list();
    while (peek().type == Token::Type::EOLINE)
        advance();

    while (peek().type == Token::Type::ELSEIF)
    {
        advance();
        expect(Token::Type::LEFT_BRACKET, "Error: expected left bracket");
        node->elseif_conds.push_back(std::move(parse_expression()));
        expect(Token::Type::RIGHT_BRACKET, "Error: expected right bracket");

        node->elseif_bodies.push_back(parse_statement_list());
        while (peek().type == Token::Type::EOLINE)
            advance();
    }

    if (peek().type == Token::Type::ELSE)
    {
        advance();
        node->else_body = parse_statement_list();
    }

    while (peek().type == Token::Type::EOLINE)
        advance();
    expect(Token::Type::ENDIF, "Error: expected endif token");
    return node;
}

std::vector<std::unique_ptr<StmtNode>> Parser::parse_statement_list()
{
    std::vector<std::unique_ptr<StmtNode>> statements;
    while (peek().type == Token::Type::EOLINE)
        advance();

    while (peek().type != Token::Type::ELSE &&
           peek().type != Token::Type::EOFILE &&
           peek().type != Token::Type::ELSEIF &&
           peek().type != Token::Type::ENDIF &&
           peek().type != Token::Type::ENDWHILE)
    {
        statements.push_back(std::move(Parser::parse_statement()));

        expect(Token::Type::EOLINE, "Error: expected newline after statement");
        while (peek().type == Token::Type::EOLINE)
            advance();
    }

    return statements;
}

std::unique_ptr<StmtNode> Parser::parse_statement()
{
    if (Parser::peek().type == Token::Type::VAR || Parser::peek().type == Token::Type::CONST)
        return Parser::parse_declaration();
    if (Parser::peek().type == Token::Type::IDENTIFIER)
        return Parser::parse_assignment();
    if (Parser::peek().type == Token::Type::IF)
        return Parser::parse_if_statement();
    if (Parser::peek().type == Token::Type::WHILE)
        return Parser::parse_while_statement();
    std::cout << "Error: unexpected token at line " << Parser::peek().line << std::endl;
    exit(1);
}

std::unique_ptr<StmtNode> Parser::parse_assignment()
{
    auto node = std::make_unique<StmtNode>();
    node->type = StmtType::ASSIGN;

    Token id = Parser::expect(Token::Type::IDENTIFIER, "Error: expected identifier");
    AssignTarget curr_target;

    curr_target.name = std::get<std::string>(id.value);
    if (peek().type == Token::Type::LEFT_SQUARE_BRACKET)
    {
        expect(Token::Type::LEFT_SQUARE_BRACKET, "Error: expected left square bracket '['");
        curr_target.index = std::move(parse_expression());
        expect(Token::Type::RIGHT_SQUARE_BRACKET, "Error: expected right square bracket ']'");
    }
    node->assign_targets.push_back(std::move(curr_target));

    while (Parser::peek().type == Token::Type::COMMA)
    {
        Parser::advance();
        id = Parser::expect(Token::Type::IDENTIFIER, "Error: expected identifier");

        AssignTarget curr_target;
        curr_target.name = std::get<std::string>(id.value);
        if (peek().type == Token::Type::LEFT_SQUARE_BRACKET)
        {
            expect(Token::Type::LEFT_SQUARE_BRACKET, "Error: expected left square bracket '['");
            curr_target.index = std::move(parse_expression());
            expect(Token::Type::RIGHT_SQUARE_BRACKET, "Error: expected right square bracket ']'");
        }
        node->assign_targets.push_back(std::move(curr_target));
    }

    Parser::expect(Token::Type::ASSIGN, "Error: expected ':='");

    std::unique_ptr<ExprNode> val = Parser::parse_value();
    node->assign_values.push_back(std::move(val));

    while (Parser::peek().type == Token::Type::COMMA)
    {
        Parser::advance();
        val = Parser::parse_value();
        node->assign_values.push_back(std::move(val));
    }
    return node;
}
std::unique_ptr<StmtNode> Parser::parse_array_declaration(int &arr_const)
{
    auto node = std::make_unique<StmtNode>();
    node -> type = StmtType::ARRAY_DECL;
    node -> arr_const = arr_const;

    expect(Token::Type::ARR, "Error: expected arr token");
    Token curr_token = peek();

    if(curr_token.type == Token::Type::OP_COMPARE && std::get<std::string>(curr_token.value) == "<")advance();
    else
    {
        std::cout << "Error: expected '<' token" << std::endl;
        exit(1);
    }

    Token type_token = Parser::advance();
    if (type_token.type == Token::Type::TYPE_BOOL)
        node->arr_decl_type = "bool";
    else if (type_token.type == Token::Type::TYPE_CHAR)
        node->arr_decl_type = "char";
    else if (type_token.type == Token::Type::TYPE_INT)
        node->arr_decl_type = "int";
    else
    {
        std::cout << "Error: unexpected token at line" << type_token.line << std::endl;
        exit(1);
    }

    expect(Token::Type::COMMA, "Error: expected colon ','");

    Token token_size = expect(Token::Type::LITERAL_INTEGER, "Error: expected literal integer token");
    node -> arr_decl_size = std::get<int>(token_size.value);
    int sz = std::get<int>(token_size.value);

    curr_token = peek();
    if(curr_token.type == Token::Type::OP_COMPARE && std::get<std::string>(curr_token.value) == ">")advance();
    else
    {
        std::cout << "Error: expected '>' token" << std::endl;
        exit(1);
    }

    Parser::expect(Token::Type::COLON, "Error: expected ':' after type");

    while (Parser::peek().type == Token::Type::IDENTIFIER)
    {
        Token id = Parser::advance();
        node->arr_decl_names.push_back(std::get<std::string>(id.value));

        std::vector<std::unique_ptr<ExprNode>> curr_init;

        if (Parser::peek().type == Token::Type::ASSIGN)
        {
            Parser::advance();
            expect(Token::Type::LEFT_CURLY_BRACKET, "Error: expected '{' token");

            for (int i = 0; i < sz; ++ i)
            {
                curr_init.push_back(std::move(parse_expression()));
                if(i < sz-1)expect(Token::Type::COMMA, "Error: expected ',' token");
            }
            expect(Token::Type::RIGHT_CURLY_BRACKET, "Error: expected '}' token");
        }

        node -> arr_decl_init.push_back(std::move(curr_init));
        if (Parser::peek().type == Token::Type::COMMA)
            Parser::advance();
        else
            break;
    }
    return node;
}




std::unique_ptr<StmtNode> Parser::parse_declaration()
{
    auto node = std::make_unique<StmtNode>();

    node -> type = StmtType::DECL;
    Token curr_token = Parser::advance();
    int arr_const;
    if (curr_token.type == Token::Type::CONST)
    {
        node->is_const = 1;
        arr_const = 1;
    }
    else
    {
        node->is_const = 0;
        arr_const = 0;
        if (curr_token.type != Token::Type::VAR)
        {
            std::cout << "Error: unexpected token at line" << curr_token.line << std::endl;
            exit(1);
        }
    }

    if(peek().type == Token::Type::ARR)
        return parse_array_declaration(arr_const);

    Token type_token = Parser::advance();
    if (type_token.type == Token::Type::TYPE_BOOL)
        node->decl_type = "bool";
    else if (type_token.type == Token::Type::TYPE_CHAR)
        node->decl_type = "char";
    else if (type_token.type == Token::Type::TYPE_INT)
        node->decl_type = "int";
    else
    {
        std::cout << "Error: unexpected token at line" << type_token.line << std::endl;
        exit(1);
    }

    Parser::expect(Token::Type::COLON, "Error: expected ':' after type");

    while (Parser::peek().type == Token::Type::IDENTIFIER)
    {
        Token id = Parser::advance();
        node->decl_names.push_back(std::get<std::string>(id.value));


        if (Parser::peek().type == Token::Type::ASSIGN)
        {
            Parser::advance();
            node->decl_init.push_back(Parser::parse_value());
        }
        else
            node->decl_init.push_back(nullptr);

        if (Parser::peek().type == Token::Type::COMMA)
            Parser::advance();
        else
            break;
    }
    return node;
}