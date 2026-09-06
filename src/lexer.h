#pragma once
#include <string>
#include <vector>
enum class TokenType
{
    VAR,
    CONST,
    ARR,
    IF,
    ELSE,
    ENDIF,
    SWAP,

    INT,
    CHAR,
    BOOL,

    IDENTIFIER,
    INTEGER,
    CHAR_LITERAL,

    ASSIGN, // :=
    EQUALS,
    LESS,
    GREATER,
    LESS_EQUAL,
    GREATER_EQUAL,

    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,

    BITWISE_AND,
    BITWISE_OR,
    BITWISE_XOR,

    SHIFT_LEFT,
    SHIFT_RIGHT,
    
    AND,
    OR,
    NOT,

    COLON,
    COMMA,
    LEFT_BRACKET,
    RIGHT_BRACKET,

    END_OF_LINE, // \n
    END_OF_FILE, // exit
    ERROR
};

struct Token
{
    TokenType type;
    std::string value;
    Token(TokenType type, const std::string& value);
    void print() const;
};

class Lexer
{
public:
    Lexer(const std::string& source);
    std:: vector < Token > tokenize();
private:
    std::string source;
};
