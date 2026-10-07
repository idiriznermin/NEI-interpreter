#include "checker.h"

#include <iostream>
#include <map>
#define maxn 100

void CompileChecker::error(const std::string& message)
{
    std::cout << "Compile error: " << message << std::endl;
    error_count ++;
}

void CompileChecker::declare(const std::string name, Symbol symbol)
{
    if(mp.count(name))
    {
        error("'" + name + "' is already declared");
        return;
    }
    mp[name] = symbol;
}

void CompileChecker::check_expr(const ExprNode *node)
{
    if(node == nullptr)
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
        else if(it->second.is_array == true)
            error("'" + node->var_name + "' is an array, use and index like " + node->var_name + "[0]");

            return;
    }
    case ExprType::ARRAY_ACCESS:
    {
        check_expr(node->arr_index.get());
        auto it = mp.find(node->arr_name);
        if(it == mp.end())
              error("array '" + node->arr_name + "' is not declared");
        else if(!it->second.is_array == false)
            error("'" + node->arr_name + "' is not an array");
        else if(node->arr_index->type == ExprType::INTEGER && (node->arr_index->integer < 0 || node->arr_index->integer >=  node->arr_index->integer >= it->second.size))
            error("index " + std::to_string(node->arr_index->integer) +
                  " is out of bounds for array '" + node->arr_name +
                  "' (size " + std::to_string(it->second.size) + ")");
        return;
    }
    case ExprType::UNARY:
    {
        check_expr(node->unary_child.get());
    }
    case ExprType::BINARY:
    {
        check_expr(node->left.get());
        check_expr(node->right.get());
        if((node->op==Operator::ARITH_DIV || node->op == Operator::ARITH_MOD) && (node->right->type == ExprType::INTEGER) && (node->right->integer == 0))
        {
            error("division by zero");
        }
        return;
    }

}