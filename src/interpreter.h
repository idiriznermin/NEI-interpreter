#pragma once

#include "ast.h"
#include "environment.h"

#include <vector>

class Interpreter
{
public:
    void run(const std::vector<std::unique_ptr<StmtNode>> &program, Environment &env);

private:
    int evaluate(const ExprNode* node, Environment &env); /// walks the expr tree
    void execute(const StmtNode* node, Environment &env);
};