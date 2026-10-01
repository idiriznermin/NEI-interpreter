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

    if (node->type == ExprType::INTEGER)
        return node->integer;

    if (node->type == ExprType::VAR)
    {
        std::string curr_name = node->var_name;
        return env.get(curr_name);
    }

    if (node->type == ExprType::ARRAY_ACCESS)
    {
        int index = evaluate(node->arr_index.get(), env);
        return env.get_array_element(node->arr_name, index);
    }

    if (node->type == ExprType::UNARY)
    {
        int child = evaluate(node->unary_child.get(), env);
        if (node->unary_op == "-")
            return (-child);
        if (node->unary_op == "!")
            return (!(child));
        if (node->unary_op == "~")
            return (~child);
        std::cout << "Error: unknown unary operation - " << node->unary_op << std::endl;
        exit(1);
    }

     if (node->type == ExprType::READ)
    {
        int value;
        if (!(std::cin >> value))
        {
            std::cout << "Error: read() expected an integer from input" << std::endl;
            exit(1);
        }
        return value;
    }

    int left_child = evaluate(node->left.get(), env);
    int right_child = evaluate(node->right.get(), env);

    if (node->op == "+")
        return left_child + right_child;
    if (node->op == "-")
        return left_child - right_child;
    if (node->op == "*")
        return left_child * right_child;

    if (node->op == "/")
    {
        if (right_child == 0)
        {
            std::cout << "Error: dividing by zero (" << left_child << " / " << right_child << ")" << std::endl;
            exit(1);
        }
        return left_child / right_child;
    }
    if (node->op == "%")
    {
        if (right_child == 0)
        {
            std::cout << "Error: modulo by zero (" << left_child << " / " << right_child << ")" << std::endl;
            exit(1);
        }
        return left_child % right_child;
    }

    if (node->op == "&")
        return (left_child & right_child);
    if (node->op == "|")
        return (left_child | right_child);
    if (node->op == "^")
        return (left_child ^ right_child);
    if (node->op == "<<")
        return (left_child << right_child);
    if (node->op == ">>")
        return (left_child >> right_child);

    if (node->op == "=")
        return (left_child == right_child) ? 1 : 0;
    if (node->op == "!=")
        return (left_child != right_child) ? 1 : 0;
    if (node->op == "<")
        return (left_child < right_child) ? 1 : 0;
    if (node->op == ">")
        return (left_child > right_child) ? 1 : 0;
    if (node->op == "<=")
        return (left_child <= right_child) ? 1 : 0;
    if (node->op == ">=")
        return (left_child >= right_child) ? 1 : 0;

    if (node->op == "&&")
        return ((left_child) && (right_child));
    if (node->op == "||")
        return ((left_child) || (right_child));

    std::cout << "Error: unknown binary operation - " << node->op << std::endl;
    exit(1);
}

void Interpreter::execute(const StmtNode *node, Environment &env)
{
    if (node == nullptr)
        return;

    if (node->type == StmtType::DECL)
    {
        bool curr_const = node->is_const;
        int sz = (int)node->decl_names.size();

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

    if (node->type == StmtType::ASSIGN)
    {
        int sz_targets = (int)node->assign_targets.size();
        int sz_values = (int)node->assign_values.size();

        if (sz_targets != sz_values)
        {
            std::cout << "Error: assignment has " << sz_targets << " target(s) but " << sz_values << " value(s)" << std::endl;
            exit(1);
        }

        std::vector<int> values;
        for (const auto &node_value : node->assign_values)
        {
            values.push_back(evaluate(node_value.get(), env));
        }
        std::vector<int> indices;
        for (const auto &target : node->assign_targets)
        {
            if (target.index)
                indices.push_back(evaluate(target.index.get(), env));
            else
                indices.push_back(0); // unused for plain variables
        }

        for (int i = 0; i < sz_targets; ++i)
        {
            const auto &target = node->assign_targets[i];
            if (target.index)
                env.assign_array_element(target.name, indices[i], values[i]);
            else
                env.assign(target.name, values[i]);
        }
        return;
    }

    if (node->type == StmtType::IF)
    {
        if (evaluate(node->if_cond.get(), env))
        {
            for (const auto &stmt_line : node->if_then_body)
            {
                execute(stmt_line.get(), env);
            }
            return;
        }

        int pos = 0;
        for (const auto &cond : node->elseif_conds)
        {
            if (evaluate(cond.get(), env))
            {
                for (const auto &stmt_line : node->elseif_bodies[pos])
                    execute(stmt_line.get(), env);
                return;
            }
            pos++;
        }
        for (const auto &stmt_line : node->else_body)
        {
            execute(stmt_line.get(), env);
        }
        return;
    }

    if (node->type == StmtType::WHILE)
    {
        while (evaluate(node->while_cond.get(), env))
        {
            for (const auto &statement : node->while_body)
                execute(statement.get(), env);
        }
        return;
    }

    if (node->type == StmtType::ARRAY_DECL)
    {
        for (int i = 0; i < (int)node->arr_decl_names.size(); ++i)
        {
            std::vector<int> values;
            for (const auto &x : node->arr_decl_init[i])
            {
                values.push_back(evaluate(x.get(), env));
            }
            env.declare_array(node->arr_decl_names[i], node->arr_decl_size, values, node->arr_const);
        }
        return;
    }

    if (node -> type == StmtType::PRINT)
    {
        std::vector<std::string> pieces;
        for (const auto &arg: node->print_args)
        {
            if (arg.expr)
                pieces.push_back(std::to_string(evaluate(arg.expr.get(), env)));
            else pieces.push_back(arg.text);
        }

        for (int i = 0; i < (int)pieces.size(); ++ i)
        {
            if(i)std::cout << " ";
            std::cout << pieces[i];
        }
        std::cout << '\n';
        return;
    }
    
    std::cout << "Error: encountered an unknown statement type during execution" << std::endl;
    exit(1);
}

void Interpreter::run(const std::vector<std::unique_ptr<StmtNode>> &program, Environment &env)
{
    for (const auto &node : program)
        execute(node.get(), env);
    return;
}
