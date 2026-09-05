#include "lexer.h"
#include <iostream>
#include <string>
#include <vector>
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
/// future fix: handle negatives and equations, handle bools
bool isInteger(const std::string& s)
{
    int sz = (int)s.size();
    if(sz == 0)return false;
    for (char sym: s)
        if(!(sym >= '0' && sym <= '9'))return false;
    return true;
}

bool isIdentifier(const std::string& s)
{
    int sz = (int)s.size();
    if(sz == 0)return false;
    if(!(s[0] >= 'a' && s[0] <= 'z') && !(s[0] >= 'A' && s[0] <= 'Z'))
        return false;
    for (char sym: s)
    {
        if(!(s[0] >= 'a' && s[0] <= 'z') && !(s[0] >= 'A' && s[0] <= 'Z') && !(s[0] >= '0' && s[0] <= '9') && s[0] != '_')
            return false;
    }
    return true;
}
std::vector < Token > Lexer::tokenize()
{
    std::vector < Token > tokens;
    int pos = 0;
    while(pos < source.size())
    {
        std::string curr = "";
        while((source[pos] != ' ') && (source[pos] != '\n') && (source[pos] != ','))
        {
            curr = curr + source[pos];
            pos ++;
        }

        std::cout << curr << std::endl;
        if(curr == "var")
            tokens.push_back(Token(TokenType::VAR, curr));
        else if(curr == "const")
            tokens.push_back(Token(TokenType::CONST, curr));
        else if(curr == "arr")
            tokens.push_back(Token((TokenType::ARR), curr));
        else if(curr == "int:")
            tokens.push_back(Token(TokenType::INT, curr));
        else if(curr == "char:")
            tokens.push_back(Token(TokenType::CHAR, curr));
        else if(curr == "bool:")
            tokens.push_back(Token(TokenType::BOOL, curr));
        else if(((int)curr.size() == 3) && (curr[0] == '\'') && (curr[2] == '\''))
            tokens.push_back(Token(TokenType::CHAR_LITERAL, curr));
        else if(isInteger(curr))
            tokens.push_back(Token(TokenType::INTEGER, curr));
        else if(curr == ":=")
            tokens.push_back(Token(TokenType::ASSIGN, curr));
        else if(curr == "=")
            tokens.push_back(Token(TokenType::EQUALS, curr));
        else if(curr == "swap")
            tokens.push_back(Token(TokenType::SWAP, curr));
        else if(isIdentifier(curr))
            tokens.push_back(Token(TokenType::IDENTIFIER, curr));

        if(source[pos] == '\n')
            tokens.push_back(Token(TokenType::END_OF_LINE, curr));
        else if(source[pos] == ',')
            tokens.push_back(Token(TokenType::COMMA, curr));

        pos ++;
    }
    return tokens;
}