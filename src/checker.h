#pragma once

#include "ast.h"

#include <map>
#include <string>
#include <vector>
#include <memory>

class CompileChecker
{
public:
    bool check(const AST& program);
private:
    struct Symbol
    {
        bool is_array;
        bool is_const;
        int size;
        Symbol(){};
        Symbol(bool is_array, bool is_const, int size):
            is_array(is_array), is_const(is_const), size(size)
        {}
    };
    std::map <std::string, Symbol> mp;
    int error_count = 0;
    void declare(const std::string& name, Symbol symbol);
    void check_expr(const ExprNode *node);
    void error(const std::string& message);
    void check_statement(const StmtNode *node);
    void check_body(const std::vector<std::unique_ptr<StmtNode>>& body);
};