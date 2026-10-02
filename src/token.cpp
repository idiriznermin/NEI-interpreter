#include "token.h"

#include <iostream>
#include <string>
#include <map>

Token::Token(Token::Type type, int line, int col):
    type(type), value(std::monostate{}), line(line), col(col)
{}

Token::Token(Token::Type type, const std::string &value, int line, int col):
    type(type), value(value), line(line), col(col)
{}

Token::Token(Token::Type type, int value, int line, int col):
    type(type), value(value), line(line), col(col)
{}

Token::Token(Token::Type type, char value, int line, int col):
    type(type), value(value), line(line), col(col)
{}

Token::Token(Token::Type type, bool value, int line, int col):
    type(type), value(value), line(line), col(col)
{}

Token::Token(Type type, Operator value, int line, int col)
    : type(type), value(value), line(line), col(col)
{}

const char* op_to_string(Token::Operator op)
{
    switch (op)
    {
    case Token::Operator::EQUALS:              return "=";
    case Token::Operator::NOT_EQUALS:          return "!=";
    case Token::Operator::LESS:                return "<";
    case Token::Operator::GREATER:             return ">";
    case Token::Operator::LESS_EQUAL:          return "<=";
    case Token::Operator::GREATER_EQUAL:       return ">=";
    case Token::Operator::ADD:                 return "+";
    case Token::Operator::SUB:                 return "-";
    case Token::Operator::MUL:                 return "*";
    case Token::Operator::DIV:                 return "/";
    case Token::Operator::MOD:                 return "%";
    case Token::Operator::BITWISE_AND:         return "&";
    case Token::Operator::BITWISE_OR:          return "|";
    case Token::Operator::BITWISE_XOR:         return "^";
    case Token::Operator::BITWISE_NOT:         return "~";
    case Token::Operator::BITWISE_LEFT_SHIFT:  return "<<";
    case Token::Operator::BITWISE_RIGHT_SHIFT: return ">>";
    case Token::Operator::LOGICAL_NOT:         return "!";
    case Token::Operator::LOGICAL_AND:         return "&&";
    case Token::Operator::LOGICAL_OR:          return "||";
    }
    return "?";
}

std::map<Token::Type, std::string> tokenStrings = {
    {Token::Type::READ, "read"},
    {Token::Type::PRINT, "print"},
    {Token::Type::VAR, "var"},
    {Token::Type::CONST, "const"},
    {Token::Type::ARR, "arr"},
    {Token::Type::IF, "if"},
    {Token::Type::ELSE, "else"},
    {Token::Type::ELSEIF, "elseif"},
    {Token::Type::ENDIF, "endif"},

    {Token::Type::WHILE, "while"},
    {Token::Type::ENDWHILE, "endwhile"},

    {Token::Type::TYPE_INT, "type_int"},
    {Token::Type::TYPE_CHAR, "type_char"},
    {Token::Type::TYPE_BOOL, "type_bool"},

    {Token::Type::IDENTIFIER, "identifier"},
    {Token::Type::LITERAL_INTEGER, "integer"},
    {Token::Type::LITERAL_CHAR, "char literal"},
    {Token::Type::LITERAL_BOOL, "bool literal"},
    {Token::Type::LITERAL_STRING, "string literal"},

    {Token::Type::ASSIGN, "assign"},
    {Token::Type::OP_COMPARE, "op_compare"},
    {Token::Type::OP_ADD, "op_add"},
    {Token::Type::OP_MUL, "op_mul"},
    {Token::Type::OP_BITWISE, "op_bitwise"},
    {Token::Type::OP_LOGICAL, "op_logical"},
    {Token::Type::OP_NOT, "op_not"},

    {Token::Type::COLON, ":"},
    {Token::Type::COMMA, ","},
    {Token::Type::LEFT_BRACKET, "("},
    {Token::Type::RIGHT_BRACKET, ")"},
    {Token::Type::LEFT_SQUARE_BRACKET, "["},
    {Token::Type::RIGHT_SQUARE_BRACKET, "]"},
    {Token::Type::LEFT_CURLY_BRACKET, "{"},
    {Token::Type::RIGHT_CURLY_BRACKET, "}"},

    {Token::Type::EOLINE, "eoline"},
    {Token::Type::EOFILE, "eofile"},
    {Token::Type::ERROR, "error"}
};

void Token::print() const
{

    std::cout << "token at line " <<  line << " and col: "<< col <<  std::endl;
    std::cout << "has type: ";
    if (tokenStrings.find(type) == tokenStrings.end())
    {
        std::cout << std::endl;
    }
    else std::cout << tokenStrings[type] << std::endl;

    std::cout << "and value: ";
    std::visit([](const auto& value) {
        using T = std::decay_t<decltype(value)>;
        if constexpr (std::is_same_v<T, std::monostate>)
            std::cout << "<empty>";
        else if constexpr (std::is_same_v<T, bool>)
            std::cout << (value ? "true" : "false");
        else if constexpr (std::is_same_v<T, char>)
            std::cout << "'" << value << "'";
        else if constexpr(std::is_same_v<T, Token::Operator>)
            std::cout << op_to_string(value);
    }, value);

    std::cout << "\n";
    std::cout << std::endl;
}