#pragma once

#include <string>

struct Message
{
    Message(const std::string& text);
    void print() const;

private:
    std::string text;
};
