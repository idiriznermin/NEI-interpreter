#include "lexer.h"
#include <iostream>
#include <string>
#include <vector>
#include <map>
Lexer::Lexer(const std::string &source):
    source(source)
{}

Token::Token(TokenType type, const std::string &value):
    type(type), value(value)
{}

void Token::print() const
{
    std::cout << value << std::endl;
}

std::map <std::string, TokenType> keywords;

void initialize_keywords()
{
    keywords["var"] = TokenType::VAR;
    keywords["const"] = TokenType::CONST;
    keywords["arr"] = TokenType::ARR;
    keywords["if"] = TokenType::IF;
    keywords["else"] = TokenType::ELSE;
    keywords["endif"] = TokenType::ENDIF;
    keywords["swap"] = TokenType::SWAP;

    keywords["int"] = TokenType::INT;
    keywords["char"] = TokenType::CHAR;
    keywords["bool"] = TokenType::BOOL;

    keywords[":="] = TokenType::ASSIGN;
    keywords["="] = TokenType::EQUALS; /// carefull with the next token
    keywords["<"] = TokenType::LESS;
    keywords[">"] = TokenType::GREATER;
    keywords["<="] = TokenType::LESS_EQUAL;
    keywords[">="] = TokenType::GREATER_EQUAL;

    keywords["+"] = TokenType::PLUS;
    keywords["-"] = TokenType::MINUS;
    keywords["*"] = TokenType::MULTIPLY;
    keywords["/"] = TokenType::DIVIDE;

    keywords["&"] = TokenType::BITWISE_AND;
    keywords["|"] = TokenType::BITWISE_OR;
    keywords["^"] = TokenType::BITWISE_XOR;

    keywords["<<"] = TokenType::SHIFT_LEFT;
    keywords[">>"] = TokenType::SHIFT_RIGHT;
    keywords["!"] = TokenType::NOT;

    keywords["&&"] = TokenType::AND;
    keywords["||"] = TokenType::OR;

    keywords[":"] = TokenType::COLON;
    keywords[","] = TokenType::COMMA;
    keywords["("] = TokenType::LEFT_BRACKET;
    keywords[")"] = TokenType::RIGHT_BRACKET;

    keywords["\n"] = TokenType::END_OF_LINE;
    keywords["exit"] = TokenType::END_OF_FILE;
}

/// 0 - empty space
/// 1 - starts a word
/// 2 - starts an integer
/// 3 - another
int determineType(char sym)
{
    if(sym == ' ')return 0;
    if(sym >= 'a' && sym <= 'z')return 1;
    if(sym >= 'A' && sym <= 'Z')return 1;
    if(sym >= '0' && sym <= '9')return 2;
    return 3;
}

bool continuesType1(char sym)
{
    if(sym >= 'a' && sym <= 'z')return true;
    if(sym >= 'A' && sym <= 'Z')return true;
    if(sym >= '0' && sym <= '9')return true;
    return (sym == '_');
}

std::vector < Token > Lexer::tokenize()
{
    initialize_keywords();
    std::vector < Token > tokens;
    int pos = 0, sz = (int)(source.size());
    while(pos < sz)
    {
        while(pos < sz && determineType(source[pos]) == 0)pos ++;
        if(pos >= sz)break;
        int current_type = determineType(source[pos]);
        std::string curr_value = "";

        if(current_type == 1)
        {
            curr_value += source[pos];
            pos ++;
            while(pos < sz && continuesType1(source[pos]))
            {
                curr_value += source[pos];
                pos ++;
            }
            if(keywords.find(curr_value) == keywords.end())
                tokens.push_back(Token(TokenType::IDENTIFIER, curr_value));
            else tokens.push_back(Token(keywords[curr_value], curr_value));
            continue;
        }
        if(current_type == 2)
        {
            curr_value += source[pos];
            pos ++;
            while(pos < sz && determineType(source[pos]) == 2)
            {
                curr_value += source[pos];
                pos ++;
            }
            tokens.push_back(Token(TokenType::INTEGER, curr_value));
            continue;
        }
        int best_matchpoint = -1;
        std::string best_match = "";
        for (int matchpoint = pos; matchpoint < (sz, pos + 5); ++ matchpoint)
        {
            curr_value += source[matchpoint];
            if(keywords.find(curr_value) == keywords.end())continue;
            best_matchpoint = matchpoint;
            best_match = curr_value;
        }
        if(best_matchpoint == -1)
        {
            std::cout << "ERROR" << std::endl;
            exit(0);
        }
        tokens.push_back(Token(keywords[best_match], best_match));
        pos = best_matchpoint + 1;
    }
    return tokens;
}