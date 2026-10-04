#pragma once

#include "ast.h"
#include "environment.h"

#include <vector>

class Runner
{
public:
    void run(const AST& program);

private:
    int evaluate(const ExprNode* node, Environment& env);
    void execute(const StmtNode* node, Environment& env);
};