#include "token.h"
#include "lexer.h"
#include "operator.h"
#include "util.h"

#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <map>

Lexer::Lexer() = default;

const std::map<std::string, Token::Type> KEYWORDS = {
    {"read", Token::Type::READ},
    {"print", Token::Type::PRINT},
    {"var", Token::Type::VAR},
    {"const", Token::Type::CONST},
    {"arr", Token::Type::ARR},
    {"if", Token::Type::IF},
    {"else", Token::Type::ELSE},
    {"endif", Token::Type::ENDIF},
    {"elseif", Token::Type::ELSEIF},
    {"while", Token::Type::WHILE},
    {"endwhile", Token::Type::ENDWHILE},

    {"int", Token::Type::TYPE_INT},
    {"char", Token::Type::TYPE_CHAR},
    {"true", Token::Type::TYPE_BOOL},
    {"false", Token::Type::TYPE_BOOL},
    {"True", Token::Type::TYPE_BOOL},
    {"False", Token::Type::TYPE_BOOL},

    {":=", Token::Type::ASSIGN},

    {"=", Token::Type::OP_COMPARE},
    {"<", Token::Type::OP_COMPARE},
    {">", Token::Type::OP_COMPARE},
    {"<=", Token::Type::OP_COMPARE},
    {">=", Token::Type::OP_COMPARE},
    {"!=", Token::Type::OP_COMPARE},

    {"+", Token::Type::OP_ADD},
    {"-", Token::Type::OP_ADD},
    {"*", Token::Type::OP_MUL},
    {"/", Token::Type::OP_MUL},
    {"%", Token::Type::OP_MUL},

    {"&", Token::Type::OP_BITWISE},
    {"|", Token::Type::OP_BITWISE},
    {"^", Token::Type::OP_BITWISE},
    {"~", Token::Type::OP_BITWISE},
    {"<<", Token::Type::OP_BITWISE},
    {">>", Token::Type::OP_BITWISE},

    {"!", Token::Type::OP_NOT},
    {"&&", Token::Type::OP_LOGICAL},
    {"||", Token::Type::OP_LOGICAL},

    {":", Token::Type::COLON},
    {",", Token::Type::COMMA},
    {"(", Token::Type::LEFT_BRACKET},
    {")", Token::Type::RIGHT_BRACKET},
    {"[", Token::Type::LEFT_SQUARE_BRACKET},
    {"]", Token::Type::RIGHT_SQUARE_BRACKET},
    {"{", Token::Type::LEFT_CURLY_BRACKET},
    {"}", Token::Type::RIGHT_CURLY_BRACKET},

    {"\n", Token::Type::EOLINE},
    {"exit", Token::Type::EOFILE}};


const std::map<std::string, Operator> OP_KEYWORDS = make_inverse_map(OPERATOR_TO_STRING);

/// 0 - empty space
/// 1 - starts a word
/// 2 - starts an integer
/// 3 - another

int determineType(char sym)
{
    if (sym == ' ')
        return 0;
    if (sym >= 'a' && sym <= 'z')
        return 1;
    if (sym >= 'A' && sym <= 'Z')
        return 1;
    if (sym >= '0' && sym <= '9')
        return 2;
    return 3;
}

bool continuesType1(char sym)
{
    if (sym >= 'a' && sym <= 'z')
        return true;
    if (sym >= 'A' && sym <= 'Z')
        return true;
    if (sym >= '0' && sym <= '9')
        return true;
    if (sym == '_')
        return true;
    return false;
}

int convert_digit(char sym)
{
    return (int)(sym - '0');
}

std::vector<Token> Lexer::tokenize(const std::string &source)
{

    std::vector<Token> tokens;
    int pos = 0, sz = (int)(source.size());

    int last_newline = 0, cnt_newlines = 0;
    while (pos < sz)
    {
        while (pos < sz && determineType(source[pos]) == 0)
        {
            pos++;
        }
        if (pos >= sz)
            break;

        if (source[pos] == '"')
        {
            pos++;
            std::string text = "";
            while (pos < sz && source[pos] != '"' && source[pos] != '\n')
            {
                text += source[pos];
                pos++;
            }
            if (pos >= sz || source[pos] == '\n')
            {
                std::cout << "Error at line " << cnt_newlines << ": unterminated string" << std::endl;
                exit(1);
            }
            pos ++;
            tokens.push_back(Token(Token::Type::LITERAL_STRING, text, cnt_newlines, pos - last_newline));
            continue;
        }
        int current_type = determineType(source[pos]);
        std::string curr_value = "";

        if (current_type == 1)
        {
            curr_value += source[pos];
            pos++;
            while (pos < sz && continuesType1(source[pos]))
            {
                curr_value += source[pos];
                pos++;
            }
            if (KEYWORDS.find(curr_value) == KEYWORDS.end())
                tokens.push_back(Token(Token::Type::IDENTIFIER, curr_value, cnt_newlines, pos - last_newline));
            else
                tokens.push_back(Token(KEYWORDS.at(curr_value), curr_value, cnt_newlines, pos - last_newline));
            continue;
        }
        if (current_type == 2)
        {
            int value = 0;
            value *= 10;
            value += convert_digit(source[pos]);
            pos++;
            while (pos < sz && determineType(source[pos]) == 2)
            {
                value *= 10;
                value += convert_digit(source[pos]);
                pos++;
            }
            tokens.push_back(Token(Token::Type::LITERAL_INTEGER, value, cnt_newlines, pos - last_newline));
            continue;
        }
        int best_matchpoint = -1;
        std::string best_match = "";
        for (int matchpoint = pos; matchpoint < std::min(sz, pos + 5); ++matchpoint)
        {
            curr_value += source[matchpoint];
            if (KEYWORDS.find(curr_value) == KEYWORDS.end())
                continue;
            best_matchpoint = matchpoint;
            best_match = curr_value;
        }
        if (best_matchpoint == -1)
        {
            std::cout << "ERROR" << std::endl;
            exit(0);
        }
        for (int ptr = pos; ptr <= best_matchpoint; ++ptr)
        {
            if (source[ptr] == '\n')
            {
                cnt_newlines++;
                last_newline = ptr;
            }
        }

        if (OP_KEYWORDS.count(best_match))
            tokens.push_back(Token(KEYWORDS.at(best_match), OP_KEYWORDS.at(best_match), cnt_newlines, pos - last_newline));
        else
            tokens.push_back(Token(KEYWORDS.at(best_match), best_match, cnt_newlines, pos - last_newline));

        pos = best_matchpoint + 1;
    }
    return tokens;
}