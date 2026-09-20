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
        case ExprType::UNARY:
            std::cout << "OPERATOR: " << node -> unary_op << '\n';
            break;
    }

    print_expr(node->left.get(), depth + 1);
    print_expr(node->right.get(), depth + 1);
    print_expr(node->unary_child.get(), depth + 1);
}

void print_stmt(const StmtNode* node, int depth = 0)
{
    if (!node)
        return;

    for (int i = 0; i < depth; ++i)
        std::cout << "  ";

    switch (node->type)
    {
        case StmtType::DECL:
        {
            std::cout << "DECL (" << (node->is_const ? "const " : "var ")
                       << node->decl_type << "):\n";
            for (size_t i = 0; i < node->decl_names.size(); ++i)
            {
                for (int j = 0; j < depth + 1; ++j)
                    std::cout << "  ";
                std::cout << "NAME: " << node->decl_names[i] << '\n';
                if (node->decl_init[i])
                    print_expr(node->decl_init[i].get(), depth + 2);
            }
            break;
        }

        case StmtType::ASSIGN:
        {
            std::cout << "ASSIGN:\n";
            for (size_t i = 0; i < node->assign_targets.size(); ++i)
            {
                for (int j = 0; j < depth + 1; ++j)
                    std::cout << "  ";
                std::cout << "TARGET: " << node->assign_targets[i] << '\n';
            }
            for (size_t i = 0; i < node->assign_values.size(); ++i)
                print_expr(node->assign_values[i].get(), depth + 1);
            break;
        }

        case StmtType::IF:
        {
            std::cout << "IF:\n";
            for (int j = 0; j < depth + 1; ++j)
                std::cout << "  ";
            std::cout << "COND:\n";
            print_expr(node->if_cond.get(), depth + 2);

            for (int j = 0; j < depth + 1; ++j)
                std::cout << "  ";
            std::cout << "THEN:\n";
            for (const auto& stmt : node->if_then_body)
                print_stmt(stmt.get(), depth + 2);

            for (size_t i = 0; i < node->elseif_conds.size(); ++i)
            {
                for (int j = 0; j < depth + 1; ++j)
                    std::cout << "  ";
                std::cout << "ELSEIF COND:\n";
                print_expr(node->elseif_conds[i].get(), depth + 2);

                for (int j = 0; j < depth + 1; ++j)
                    std::cout << "  ";
                std::cout << "ELSEIF BODY:\n";
                for (const auto& stmt : node->elseif_bodies[i])
                    print_stmt(stmt.get(), depth + 2);
            }

            if (!node->else_body.empty())
            {
                for (int j = 0; j < depth + 1; ++j)
                    std::cout << "  ";
                std::cout << "ELSE:\n";
                for (const auto& stmt : node->else_body)
                    print_stmt(stmt.get(), depth + 2);
            }
            break;
        }
    }
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
   Parser parser(tokens);
auto program = parser.parse_program();
for (const auto& stmt : program)
    print_stmt(stmt.get());
    return 0;
}
