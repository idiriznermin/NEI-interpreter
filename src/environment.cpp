#include "environment.h"

#include <iostream>
#include <vector>
void Environment::declare(std::string name, int value, bool is_const)
{
    if (variables.count(name))
    {
        std::cout << "Error: variable " << name << " already declared" << std::endl;
        exit(1);
    }
    if (arrays.count(name))
    {
        std::cout << "Error: '" << name << "' is an array, use an index like " << name << "[0]" << std::endl;
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
        if (arrays.count(name))
        {
            std::cout << "Error: '" << name << "' is an array, use an index like " << name << "[0]" << std::endl;
            exit(1);
        }
        std::cout << "Error: variable " << name << " not declared" << std::endl;
        exit(1);
    }
}

void Environment::assign(std::string name, int value)
{
    if (variables.count(name))
    {
        if (variables[name].is_const == 1)
        {
            std::cout << "Error: cannot assign to " << name << " - it is declared const" << std::endl;
            exit(1);
        }
        else
            variables[name].value = value;
    }
    else
    {
        if (arrays.count(name))
        {
            std::cout << "Error: '" << name << "' is an array, use an index like " << name << "[0]" << std::endl;
            exit(1);
        }
        std::cout << "Error: cannot assign to " << name << " - it is not declared" << std::endl;
        exit(1);
    }
}

void Environment::declare_array(std::string name, int size, std::vector<int> values, bool is_const)
{
    if (size <= 0)
    {
        std::cout << "Error: invalid array size " << std::endl;
        exit(1);
    }

    if (values.empty() == true)
        values.assign(size, 0);
    else if ((int)values.size() != size)
    {
        std::cout << "Error: incorrect element count in declaration of array '" << name << "'" << std::endl;
        exit(1);
    }

    if (variables.count(name) || arrays.count(name))
    {
        std::cout << "Error: '" << name << "' is already declared" << std::endl;
        exit(1);
    }

    arr_info array;
    array = arr_info(is_const, size, values);
    arrays[name] = array;
    return;
}

int Environment::get_array_element(std::string name, int index)
{
    if (arrays.count(name))
    {
        int size = arrays[name].size;
        if (index < 0 || index >= size)
        {
            std::cout << "Error: index " << index << " out of bound for array '" << name << "'" << std::endl;
            exit(1);
        }
        return arrays[name].values[index];
    }
    else
    {
        if (variables.count(name))
        {
            std::cout << "Error: " << name << " is a variable, not an array" << std::endl;
            exit(1);
        }
        std::cout << "Error: array " << name << " does not exist" << std::endl;
        exit(1);
    }
    return 0;
}

void Environment::assign_array_element(std::string name, int index, int value)
{
    if (arrays.count(name))
    {
        int size = arrays[name].size;
        if (index < 0 || index >= size)
        {
            std::cout << "Error: index " << index << " out of bound for array '" << name << "'" << std::endl;
            exit(1);
        }
        if (arrays[name].is_const == true)
        {
            std::cout << "Error: cannot assign to an element of const array '" << name << "'" << std::endl;
            exit(1);
        }
        arrays[name].values[index] = value;
        return;
    }
    else
    {
        if (variables.count(name))
        {
            std::cout << "Error: " << name << " is a variable, not an array" << std::endl;
            exit(1);
        }
        std::cout << "Error: array " << name << " does not exist" << std::endl;
        exit(1);
    }
    return;
}