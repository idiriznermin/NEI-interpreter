#pragma once

#include "token.h"
#include "ast.h"

#include <vector>
#include <iostream>

class Parser
{

public:
    Parser(std::vector<Token> tokens) : tokens(std::move(tokens)), pos(0)
    {
    }
    bool check_bitwise(Token::Operator op);
    std::unique_ptr<ExprNode> parse_expression();
    std::unique_ptr<ExprNode> parse_bit_or();
    std::unique_ptr<ExprNode> parse_bit_xor();
    std::unique_ptr<ExprNode> parse_bit_and();
    std::unique_ptr<ExprNode> parse_shift();
    std::unique_ptr<ExprNode> parse_factor();
    std::unique_ptr<ExprNode> parse_value();
    std::unique_ptr<ExprNode> parse_unary();
    std::unique_ptr<ExprNode> parse_term();
    std::unique_ptr<ExprNode> parse_arith();
    std::unique_ptr<ExprNode> parse_comparison();
    std::unique_ptr<ExprNode> parse_logical();

    std::unique_ptr<StmtNode> parse_declaration();
    std::unique_ptr<StmtNode> parse_array_declaration(int &arr_const);
    std::unique_ptr<StmtNode> parse_assignment();
    std::unique_ptr<StmtNode> parse_statement();
    std::vector<std::unique_ptr<StmtNode>> parse_statement_list();
    std::unique_ptr<StmtNode> parse_if_statement();
    std::unique_ptr<StmtNode> parse_while_statement();
    std::vector<std::unique_ptr<StmtNode>> parse_program();
    PrintArg parse_print_arg();
    std::unique_ptr<StmtNode> parse_print_statement();

private:
    std::vector<Token> tokens;
    int pos;

    Token &peek()
    {
        if (pos >= (int)tokens.size())
            return tokens.back();
        return tokens[pos];
    }

    Token &advance()
    {
        return tokens[pos++];
    }

    bool check(Token::Type t)
    {
        return (t == peek().type);
    }

    Token expect(Token::Type t, const std::string &err_message)
    {
        if (!check(t))
        {
            std::cout << "Error occured at line " << peek().line << std::endl;
            std::cout << err_message << std::endl;
            exit(1);
        }
        return advance();
    }

};