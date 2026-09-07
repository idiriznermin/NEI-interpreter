#pragma once

#include <string>
#include<variant>

class Token
{

public:
    enum class Type
    {
        VAR,
        CONST,
        ARR,
        IF,
        ELSE,
        ENDIF,
        SWAP,

        TYPE_INT,
        TYPE_CHAR,
        TYPE_BOOL,

        IDENTIFIER,
        LITERAL_INTEGER,
        LITERAL_CHAR,
        LITERAL_BOOL,

        ASSIGN, // :=
        EQUALS,
        LESS,
        GREATER,
        LESS_EQUAL,
        GREATER_EQUAL,

        ADD,
        SUB,
        MUL,
        DIV,
        MOD,

        BITWISE_AND,
        BITWISE_OR,
        BITWISE_XOR,
        BITWISE_NOT,

        BITWISE_LEFT_SHIFT,
        BITWISE_RIGHT_SHIFT,

        LOGICAL_AND,
        LOGICAL_OR,
        LOGICAL_NOT,

        COLON,
        COMMA,
        LEFT_BRACKET,
        RIGHT_BRACKET,

        EOLINE, // \n
        EOFILE,
        ERROR
    };

    Token::Type type;

    std::variant<
        std::monostate,
        std::string,
        int,
        char,
        bool
    > value;

    int line, col;

    Token(Type type, int line, int col);
    Token(Type type, const std::string& value, int line, int col);
    Token(Type type, int value, int line, int col);
    Token(Type type, char value, int line, int col);
    Token(Type type, bool value, int line, int col);

    void print() const;

};

