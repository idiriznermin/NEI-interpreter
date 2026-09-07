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

void Token::print() const
{
    std::cout << value << " @ line: " << line << " col: "<< col <<  std::endl;
}

