#include "message.h"

#include <iostream>

Message::Message(const std::string& text):
    text(text)
{}

void Message::print() const
{
    std::cout << text << std::endl;
}
