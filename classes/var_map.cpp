#include <assert.h>
#include <algorithm>

#include <iostream>
#include "var_map.h"

Var* var_map::getVariable(std::string name) {
    return this->variables[name];
}

void var_map::setVariable(std::string name) {
    this->variables[name] = new Var(name, this->memory_pointer);
    this->memory_pointer++;
}

Var* var_map::getNumberAsVariable(int value) {
    Var* return_Var = new Var(value);
    // std::cout << "Get number as variable 3\n"; 
    // std::cout << value << std::endl;
    // std::cout << return_Var->getConstValue() << std::endl;
    return return_Var;
}

