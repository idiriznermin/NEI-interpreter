#pragma once

#include "token.h"
#include "operator.h"

#include<memory>
#include<string>
#include<vector>

enum class ExprType {
    INTEGER,
    VAR,
    ARRAY_ACCESS,
    BINARY,
    UNARY,
    READ
};

struct ExprNode
{
    ExprType type;
    int integer;
    std::string var_name;
    std::string arr_name;
    std::unique_ptr<ExprNode> arr_index;
    Operator op;
    std::unique_ptr<ExprNode> left;
    std::unique_ptr<ExprNode> right;
    Operator unary_op;
    std::unique_ptr<ExprNode> unary_child;
};

enum class StmtType {
    DECL,
    ARRAY_DECL,
    ASSIGN,
    IF,
    WHILE,
    PRINT
};

struct AssignTarget
{
    std::string name;
    std::unique_ptr<ExprNode> index; /// if nullptr -> plain variable
};

struct PrintArg
{
    std::string text;
    std::unique_ptr<ExprNode> expr;
};

struct StmtNode
{
    StmtType type;

    /// type = DECL
    bool is_const;

    std::string decl_type;
    std::vector<std::string> decl_names;
    std::vector<std::unique_ptr<ExprNode>> decl_init;

    /// type == ARRAY_DECL
    int arr_const;
    std::string arr_decl_type;
    std::vector<std::string> arr_decl_names;
    int arr_decl_size;
    std::vector<std::vector<std::unique_ptr<ExprNode>>> arr_decl_init;

    /// type = ASSIGN
    std::vector<AssignTarget> assign_targets;
    std::vector<std::unique_ptr<ExprNode>> assign_values;

    /// type = IF
    std::unique_ptr<ExprNode> if_cond;
    std::vector<std::unique_ptr<StmtNode>> if_then_body;
    std::vector<std::unique_ptr<ExprNode>> elseif_conds;
    std::vector<std::vector<std::unique_ptr<StmtNode>>> elseif_bodies;
    std::vector<std::unique_ptr<StmtNode>> else_body;

    /// type = WHILE
    std::unique_ptr<ExprNode> while_cond;
    std::vector<std::unique_ptr<StmtNode>> while_body;

    /// type = PRINT
    std::vector<PrintArg> print_args;
};

class AST
{
public:
   std::vector<std::unique_ptr<StmtNode>> nodes;
    AST(std::vector<std::unique_ptr<StmtNode>> nodes):
        nodes(std::move(nodes))
    {}
};
