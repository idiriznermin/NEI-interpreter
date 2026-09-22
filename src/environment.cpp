#include "environment.h"

#include <iostream>

void Environment::declare(std::string name, int value, bool is_const)
{
    if (variables.count(name))
    {
        std::cout << "Error: variable " << name << " already declared" << std::endl;
        exit(1);
    }
    variables[name] = var_info(value, is_const);
}

int Environment::get(std::string name)
{
    if (variables.count(name))
        return variables[name].value;
    else
    {
        std::cout << "Error: variable " << name << " not declared" << std::endl;
        exit(1);
    }
}

void Environment::assign(std::string name, int value)
{
    if (variables.count(name))
    {
        if(variables[name].is_const == 1)
        {
            std::cout << "Error: cannot assign to " << name << " - it is declared const" << std::endl;
            exit(1);
        }
        else variables[name].value = value;
    }
    else
    {
        std::cout << "Error: cannot assign to " << name << " - it is not declared" << std::endl;
        exit(1);
    }
}