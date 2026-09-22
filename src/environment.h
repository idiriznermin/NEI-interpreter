#pragma once

#include <map>
#include <string>

struct var_info
{
    int value;
    bool is_const;

    var_info(){};
    var_info(int value, bool is_const):
        value(value), is_const(is_const)
    {}
};

class Environment
{
public:
    void declare(std::string name, int value, bool is_const);
    int get(std::string name);
    void assign(std::string name, int value);
    std::map<std::string, var_info> variables;
};