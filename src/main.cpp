#include "message.h"
#include "token.h"
#include "lexer.h"
#include "parser.h"
#include "ast.h"
#include "environment.h"
#include "interpreter.h"

#include <iostream>
#include <string>

void indent(int depth)
{
    for (int i = 0; i < depth; ++i)
        std::cout << "  ";
}

void print_expr(const ExprNode *node, int depth = 0)
{
    if (!node)
        return;

    indent(depth);

    switch (node->type)
    {
    case ExprType::INTEGER:
        std::cout << "INTEGER: " << node->integer << '\n';
        break;
    case ExprType::VAR:
        std::cout << "VARIABLE: " << node->var_name << '\n';
        break;
    case ExprType::ARRAY_ACCESS:
        std::cout << "ARRAY ACCESS: " << node->arr_name << '\n';
        indent(depth + 1);
        std::cout << "INDEX:\n";
        print_expr(node->arr_index.get(), depth + 2);
        return; // index already printed; skip the generic child calls below
    case ExprType::BINARY:
        std::cout << "OPERATOR: " << node->op << '\n';
        break;
    case ExprType::UNARY:
        std::cout << "UNARY OPERATOR: " << node->unary_op << '\n';
        break;
    }

    print_expr(node->left.get(), depth + 1);
    print_expr(node->right.get(), depth + 1);
    print_expr(node->unary_child.get(), depth + 1);
}

void print_body(const std::vector<std::unique_ptr<StmtNode>> &body, int depth);

void print_stmt(const StmtNode *node, int depth = 0)
{
    if (!node)
        return;

    indent(depth);

    switch (node->type)
    {
    case StmtType::DECL:
    {
        std::cout << "DECL (" << (node->is_const ? "const " : "var ")
                  << node->decl_type << "):\n";
        for (size_t i = 0; i < node->decl_names.size(); ++i)
        {
            indent(depth + 1);
            std::cout << "NAME: " << node->decl_names[i] << '\n';
            if (node->decl_init[i])
                print_expr(node->decl_init[i].get(), depth + 2);
        }
        break;
    }

    case StmtType::ARRAY_DECL:
    {
        std::cout << "ARRAY DECL (" << (node->arr_const ? "const " : "var ")
                  << "arr<" << node->arr_decl_type << ", " << node->arr_decl_size << ">):\n";
        for (size_t i = 0; i < node->arr_decl_names.size(); ++i)
        {
            indent(depth + 1);
            std::cout << "NAME: " << node->arr_decl_names[i];
            if (node->arr_decl_init[i].empty())
            {
                std::cout << " (no initializer)\n";
                continue;
            }
            std::cout << '\n';
            for (size_t j = 0; j < node->arr_decl_init[i].size(); ++j)
            {
                indent(depth + 2);
                std::cout << "[" << j << "]:\n";
                print_expr(node->arr_decl_init[i][j].get(), depth + 3);
            }
        }
        break;
    }

    case StmtType::ASSIGN:
    {
        std::cout << "ASSIGN:\n";
        for (const auto &target : node->assign_targets)
        {
            indent(depth + 1);
            std::cout << "TARGET: " << target.name << '\n';
            if (target.index)
            {
                indent(depth + 2);
                std::cout << "INDEX:\n";
                print_expr(target.index.get(), depth + 3);
            }
        }
        indent(depth + 1);
        std::cout << "VALUES:\n";
        for (const auto &value : node->assign_values)
            print_expr(value.get(), depth + 2);
        break;
    }

    case StmtType::IF:
    {
        std::cout << "IF:\n";
        indent(depth + 1);
        std::cout << "COND:\n";
        print_expr(node->if_cond.get(), depth + 2);

        indent(depth + 1);
        std::cout << "THEN:\n";
        print_body(node->if_then_body, depth + 2);

        for (size_t i = 0; i < node->elseif_conds.size(); ++i)
        {
            indent(depth + 1);
            std::cout << "ELSEIF COND:\n";
            print_expr(node->elseif_conds[i].get(), depth + 2);

            indent(depth + 1);
            std::cout << "ELSEIF BODY:\n";
            print_body(node->elseif_bodies[i], depth + 2);
        }

        if (!node->else_body.empty())
        {
            indent(depth + 1);
            std::cout << "ELSE:\n";
            print_body(node->else_body, depth + 2);
        }
        break;
    }

    case StmtType::WHILE:
    {
        std::cout << "WHILE:\n";
        indent(depth + 1);
        std::cout << "COND:\n";
        print_expr(node->while_cond.get(), depth + 2);

        indent(depth + 1);
        std::cout << "BODY:\n";
        print_body(node->while_body, depth + 2);
        break;
    }
    }
}

void print_body(const std::vector<std::unique_ptr<StmtNode>> &body, int depth)
{
    for (const auto &stmt : body)
        print_stmt(stmt.get(), depth);
}

int main()
{
    /// g++ main.cpp environment.cpp interpreter.cpp lexer.cpp message.cpp parser_exprs.cpp parser_stmts.cpp token.cpp -o nei.exe
    // Message msg("NYA Hello World");
    // msg.print();

    std::string source(
        std::istreambuf_iterator<char>(std::cin),
        std::istreambuf_iterator<char>());

    Lexer lexer;
    std::vector<Token> tokens = lexer.tokenize(source);

    // uncomment if you want to see the tokens too
    // for (Token t : tokens)
    //     t.print();

    Parser parser(tokens);
    auto program = parser.parse_program();

    for (const auto &stmt : program)
        print_stmt(stmt.get());
    
    Environment env;
    Interpreter inter;
    inter.run(program, env);

    for (const auto &[name, info] : env.get_variables())
        std::cout << name << " = " << info.value << (info.is_const ? " (const)" : "") << '\n';

    for (const auto &[name, info] : env.get_arrays())
    {
        std::cout << name << " = {";
        for (size_t i = 0; i < info.values.size(); ++i)
            std::cout << (i ? ", " : "") << info.values[i];
        std::cout << "}" << (info.is_const ? " (const)" : "") << '\n';
    }
    /*std::string source(
        std::istreambuf_iterator<char>(std::cin),
        std::istreambuf_iterator<char>());

    Lexer lexer;
    std::vector<Token> tokens = lexer.tokenize(source);

    for (Token t : tokens)
    {
        t.print();
    }
    Parser parser(tokens);
    auto program = parser.parse_program();
    Environment env;
    Interpreter inter;
    inter.run(program, env);
    for (auto &[key, value]: env.variables)
        std::cout << key << " " << value.value << " and " << value.is_const << std::endl;
*/
    return 0;
}
