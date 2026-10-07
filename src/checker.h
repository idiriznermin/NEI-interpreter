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
    };
    std::map <std::string, Symbol> mp;
    int error_count = 0;
    void declare(const std::string name, Symbol symbol);
    void check_expr(const ExprNode *node);
    void error(const std::string& message);
    void check_statement(const StmtNode *node);
    void check_statement_list(const std::vector<std::unique_ptr<StmtNode>> nodes);
};