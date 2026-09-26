#pragma once

#include <map>
#include <string>
#include<vector>

struct var_info
{
    int value;
    bool is_const;

    var_info(){};
    var_info(int value, bool is_const):
        value(value), is_const(is_const)
    {}
};

struct arr_info
{
    bool is_const;
    int size;
    std::vector <int> values;

    arr_info(){};
    arr_info(bool is_const, int size, std::vector<int> values):
         is_const(is_const), size(size), values(std::move(values))
    {}
};

class Environment
{
public:
    void declare(std::string name, int value, bool is_const);
    int get(std::string name);
    void assign(std::string name, int value);

    void declare_array(std::string name, int size, std::vector<int> values, bool is_const);
    int get_array_element(std::string name, int index);
    void assign_array_element(std::string name, int index, int value);

    const std::map<std::string, var_info>& get_variables() const { return variables; }
    const std::map<std::string, arr_info>& get_arrays() const { return arrays; }
private:
    std::map<std::string, var_info> variables;
    std::map<std::string, arr_info> arrays;
};