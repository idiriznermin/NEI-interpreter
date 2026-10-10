#include "checker.h"

#include <iostream>
#include <map>

void CompileChecker::error(const std::string &message)
{
    std::cout << "Compile error: " << message << std::endl;
    error_count++;
}

void CompileChecker::declare(const std::string &name, Symbol symbol)
{
    if (mp.count(name))
    {
        error("'" + name + "' is already declared");
        return;
    }
    mp[name] = symbol;
}

void CompileChecker::check_expr(const ExprNode *node)
{
    if (node == nullptr)
        return;
    switch (node->type)
    {
    case ExprType::INTEGER:
    case ExprType::READ:
        return;
    case ExprType::VAR:
    {
        auto it = mp.find(node->var_name);
        if (it == mp.end())
            error("'" + node->var_name + "' is not declared");
        else if (it->second.is_array == true)
            error("'" + node->var_name + "' is an array, use an index like " + node->var_name + "[0]");
        return;
    }
    case ExprType::ARRAY_ACCESS:
    {
        check_expr(node->arr_index.get());
        auto it = mp.find(node->arr_name);
        if (it == mp.end())
            error("array '" + node->arr_name + "' is not declared");
        else if (!it->second.is_array == true)
            error("'" + node->arr_name + "' is not an array");
        else if (node->arr_index->type == ExprType::INTEGER
            && (node->arr_index->integer < 0 ||
                (node->arr_index->integer >= it->second.size)))
            error("index " + std::to_string(node->arr_index->integer) +
                  " is out of bounds for array '" + node->arr_name +
                  "' (size " + std::to_string(it->second.size) + ")");
        return;
    }
    case ExprType::UNARY:
    {
        check_expr(node->unary_child.get());
        return;
    }
    case ExprType::BINARY:
    {
        check_expr(node->left.get());
        check_expr(node->right.get());
        if ((node->op == Operator::ARITH_DIV || node->op == Operator::ARITH_MOD) &&
            (node->right->type == ExprType::INTEGER) &&
            (node->right->integer == 0))
        {
            error("division by zero");
        }
        return;
    }
    }
}

void CompileChecker::check_body(const std::vector<std::unique_ptr<StmtNode>>& body)
{
    for (const auto& stmt: body)
        check_statement(stmt.get());
    return;
}

void CompileChecker::check_statement(const StmtNode *node)
{
    if(node == nullptr)return;
    switch (node->type)
    {
    case StmtType::DECL:

        for (const auto& init: node->decl_init)
            check_expr(init.get());
        for (const auto& name: node->decl_names)
        {
            Symbol symbol = Symbol{false, (bool)node->is_const, 0};
            declare(name, symbol);
        }
        return;
    case StmtType::ARRAY_DECL:
        if (node->arr_decl_size <= 0)
            error("array size must be at least 1");
        for (int i = 0; i < (int)(node->arr_decl_names.size()); ++ i)
        {
            const auto &init = node->arr_decl_init[i];

            for (const auto& value: init)
                check_expr(value.get());

            if(!init.empty() && ((int)init.size() != node->arr_decl_size))
            {
                error("array '" + node->arr_decl_names[i] + "' has size " +
                      std::to_string(node->arr_decl_size) + " but " +
                      std::to_string(init.size()) + " initial value(s)");
            }

            declare(node->arr_decl_names[i], Symbol{true, (bool)node->arr_const, node->arr_decl_size});
        }
        return;
    case StmtType::ASSIGN:
        if((int)node->assign_targets.size() != (int)node->assign_values.size())
            error("assignment has " + std::to_string(node->assign_targets.size()) +
                  " target(s) but " + std::to_string(node->assign_values.size()) + " value(s)");

        for (const auto& values: node->assign_values)
            check_expr(values.get());

        for (const auto& target: node->assign_targets)
        {
            check_expr(target.index.get());
            auto it = mp.find(target.name);

            if(it == mp.end())
            {
                error("cannot assign to '" + target.name + "', it is not declared");
                continue;
            }

            const Symbol symbol = it->second;
            if (target.index && !symbol.is_array)
                error("'" + target.name + "' is a variable, not an array");
            else if (!target.index && symbol.is_array)
                error("'" + target.name + "' is an array, use an index like " + target.name + "[0]");
            else if (symbol.is_const)
                error("cannot assign to '" + target.name + "', it is declared const");
            else if (target.index && target.index->type == ExprType::INTEGER &&
                     target.index->integer >= symbol.size)
                error("index " + std::to_string(target.index->integer) +
                      " is out of bounds for array '" + target.name +
                      "' (size " + std::to_string(symbol.size) + ")");
        }
        return;
    case StmtType::IF:
        check_expr(node->if_cond.get());
        check_body(node->if_then_body);
        for (int i = 0; i < (int)node->elseif_conds.size(); ++ i)
        {
            check_expr(node->elseif_conds[i].get());
            check_body(node->elseif_bodies[i]);
        }
        check_body(node->else_body);
        return;
    case StmtType::WHILE:
        check_expr(node->while_cond.get());
        check_body(node->while_body);
        return;
    case StmtType::PRINT:
        for (const auto &arg : node->print_args)
            check_expr(arg.expr.get());
        return;
    }
}

bool CompileChecker::check(const AST& program)
{
    check_body(program.nodes);

    if(error_count > 0)
        std::cout<< error_count << " error(s), program did not run" << std::endl;
     return error_count == 0;
}

