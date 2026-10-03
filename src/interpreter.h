#pragma once

#include "ast.h"
#include "environment.h"

#include <vector>

class Interpreter
{
public:
    void run(const std::vector<std::unique_ptr<StmtNode>> &program, Environment &env);

private:
    int evaluate(const ExprNode* node, Environment &env);
    void execute(const StmtNode* node, Environment &env);
};