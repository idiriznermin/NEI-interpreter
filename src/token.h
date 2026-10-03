#pragma once

#include "operator.h"

#include <string>
#include<variant>

class Token
{

public:
    enum class Type
    {

        READ,
        PRINT,
        VAR,
        CONST,
        ARR,
        IF,
        ELSE,
        ELSEIF,
        ENDIF,

        WHILE,
        ENDWHILE,

        TYPE_INT,
        TYPE_CHAR,
        TYPE_BOOL,

        IDENTIFIER,
        LITERAL_INTEGER,
        LITERAL_CHAR,
        LITERAL_BOOL,
        LITERAL_STRING,

        ASSIGN, // :=
        OP_COMPARE,
        OP_ADD,
        OP_MUL,
        OP_BITWISE,
        OP_LOGICAL,
        OP_NOT,

        COLON,
        COMMA,
        LEFT_BRACKET,
        RIGHT_BRACKET,
        LEFT_SQUARE_BRACKET,
        RIGHT_SQUARE_BRACKET,
        LEFT_CURLY_BRACKET,
        RIGHT_CURLY_BRACKET,

        EOLINE, // \n
        EOFILE
    };


    Token::Type type;

    std::variant<
        std::monostate,
        std::string,
        int,
        char,
        bool,
        Operator
    > value;

    int line, col;

    Token(Type type, int line, int col);
    Token(Type type, const std::string& value, int line, int col);
    Token(Type type, int value, int line, int col);
    Token(Type type, char value, int line, int col);
    Token(Type type, bool value, int line, int col);
    Token(Type type, Operator value, int line, int col);

    void print() const;

};
