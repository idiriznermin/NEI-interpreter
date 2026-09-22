#include "interpreter.h"

#include "ast.h"
#include "parser.h"
#include "environment.h"

#include <vector>
#include <map>
#include <string>
#include <iostream>

int Interpreter::evaluate(const ExprNode *node, Environment &env)
{
    if (node == nullptr)
        return 0;

    if (node -> type == ExprType::INTEGER)
        return node->integer;

    if (node -> type == ExprType::VAR)
    {
        std::string curr_name = node->var_name;
        return env.get(curr_name);
    }

    if (node -> type == ExprType::UNARY)
    {
        int child = evaluate(node -> unary_child.get(), env);
        if (node -> unary_op == "-")
            return (-child);
        if (node -> unary_op == "!")
            return (!(child));
        if (node -> unary_op == "~")
            return (~child);
        std::cout << "Error: unknown unary operation - " << node->unary_op << std::endl;
        exit(1);
    }

    int left_child = evaluate(node -> left.get(), env);
    int right_child = evaluate(node -> right.get(), env);

    if (node -> op == "+")
        return left_child + right_child;
    if (node -> op == "-")
        return left_child - right_child;
    if (node -> op == "*")
        return left_child * right_child;

    if (node -> op == "/")
    {
        if (right_child == 0)
        {
            std::cout << "Error: dividing by zero (" << left_child << " / " << right_child << ")" << std::endl;
            exit(1);
        }
        return left_child / right_child;
    }
    if (node -> op == "%")
    {
        if (right_child == 0)
        {
            std::cout << "Error: modulo by zero (" << left_child << " / " << right_child << ")" << std::endl;
            exit(1);
        }
        return left_child % right_child;
    }

    if (node -> op == "&")
        return (left_child & right_child);
    if (node -> op == "|")
        return (left_child | right_child);
    if (node -> op == "^")
        return (left_child ^ right_child);
    if (node -> op == "<<")
        return (left_child << right_child);
    if (node -> op == ">>")
        return (left_child >> right_child);

    if (node -> op == "=")
        return (left_child == right_child) ? 1 : 0;
    if (node -> op == "!=")
        return (left_child != right_child) ? 1 : 0;
    if (node -> op == "<")
        return (left_child < right_child) ? 1 : 0;
    if (node -> op == ">")
        return (left_child > right_child) ? 1 : 0;
    if (node -> op == "<=")
        return (left_child <= right_child) ? 1 : 0;
    if (node -> op == ">=")
        return (left_child >= right_child) ? 1 : 0;

    if (node -> op == "&&")
        return ((left_child) && (right_child));
    if (node -> op == "||")
        return ((left_child) || (right_child));

    std::cout << "Error: unknown binary operation - " << node -> op << std::endl;
    exit(1);
}

void Interpreter::execute(const StmtNode *node, Environment &env)
{
    if (node == nullptr)
        return;

    if (node -> type == StmtType::DECL)
    {
        bool curr_const = node -> is_const;
        int sz = (int)node -> decl_names.size();

        std::vector<int> values;
        for (const auto &node_init : node->decl_init)
        {
            if (node_init == nullptr)
                values.push_back(0);
            else
                values.push_back(evaluate(node_init.get(), env));
        }
        for (int i = 0; i < sz; ++i)
            env.declare(node->decl_names[i], values[i], curr_const);
        return;
    }

    if (node -> type == StmtType::ASSIGN)
    {
        int sz_targets = (int)node -> assign_targets.size();
        int sz_values = (int)node -> assign_values.size();

        if (sz_targets != sz_values)
        {
            std::cout << "Error: assignment has " << sz_targets <<  " target(s) but " << sz_values << " value(s)" << std::endl;
            exit(1);
        }

        std::vector<int> values;
        for (const auto &node_value : node -> assign_values)
            values.push_back(evaluate(node_value.get(), env));
        for (int i = 0; i < sz_targets; ++ i)
            env.assign(node -> assign_targets[i], values[i]);
        return;
    }

    if (node -> type == StmtType::IF)
    {
        if (evaluate(node -> if_cond.get(), env))
        {
            for (const auto &stmt_line : node -> if_then_body)
            {
                execute(stmt_line.get(), env);
            }
            return;
        }

        int pos = 0;
        for (const auto &cond : node -> elseif_conds)
        {
            if (evaluate(cond.get(), env))
            {
                for (const auto &stmt_line : node -> elseif_bodies[pos])
                    execute(stmt_line.get(), env);
                return;
            }
            pos ++;
        }
        for (const auto &stmt_line : node -> else_body)
        {
            execute(stmt_line.get(), env);
        }
        return;
    }

    std::cout << "Error: encountered an unknown statement type during execution"<< std::endl;
    exit(1);
}

void Interpreter::run(const std::vector<std::unique_ptr<StmtNode>> &program, Environment &env)
{
    for (const auto& node: program)
        execute(node.get(), env);
    return;
}
