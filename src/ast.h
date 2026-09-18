#pragma once

#include<memory>
#include<string>
#include<vector>

enum class ExprType {
    INTEGER,
    VAR,
    BINARY,
    UNARY
};

struct ExprNode
{
    ExprType type;
    int integer;
    std::string var_name;
    std::string op;
    std::unique_ptr<ExprNode> left;
    std::unique_ptr<ExprNode> right;
    std::string unary_op;
    std::unique_ptr<ExprNode> unary_child;
};

enum class StmtType {
    DECL,
    ASSIGN,
    IF
};

struct StmtNode
{
    StmtType type;

    /// type = DECL
    bool is_const;
    std::string decl_type;
    std::vector<std::string> decl_names;
    std::vector<std::unique_ptr<ExprNode>> decl_init;

    /// type = ASSIGN
    std::vector<std::string> assign_targets;
    std::vector<std::unique_ptr<ExprNode>> assign_values;

    /// type = IF
    std::unique_ptr<ExprNode> if_cond;
    std::vector<std::unique_ptr<StmtNode>> if_then_body;
    std::vector<std::unique_ptr<ExprNode>> elseif_conds;
    std::vector<std::vector<std::unique_ptr<StmtNode>>> elseif_bodies;
    std::vector<std::unique_ptr<StmtNode>> else_body;
};