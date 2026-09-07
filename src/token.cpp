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

std::map<Token::Type, std::string> tokenStrings = {
    {Token::Type::VAR, "var"},
    {Token::Type::CONST, "const"},
    {Token::Type::ARR, "arr"},
    {Token::Type::IF, "if"},
    {Token::Type::ELSE, "else"},
    {Token::Type::ENDIF, "endif"},

    {Token::Type::TYPE_INT, "int"},
    {Token::Type::TYPE_CHAR, "char"},
    {Token::Type::TYPE_BOOL, "bool"},

    {Token::Type::IDENTIFIER, "identifier"},
    {Token::Type::LITERAL_INTEGER, "integer"},
    {Token::Type::LITERAL_CHAR, "char literal"},
    {Token::Type::LITERAL_BOOL, "bool literal"},

    {Token::Type::ASSIGN, ":="},
    {Token::Type::OPERATOR, "operator"},

    {Token::Type::COLON, ":"},
    {Token::Type::COMMA, ","},
    {Token::Type::LEFT_BRACKET, "("},
    {Token::Type::RIGHT_BRACKET, ")"},

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
        else
            std::cout << value; // std::string, int
    }, value);

    std::cout << "\n";
    std::cout << std::endl;
}