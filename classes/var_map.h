#ifndef VAR_MAP_H
#define VAR_MAP_H

#include <map>
#include <vector>
#include <string>
#include "variable.h"

class var_map {
public:
    var_map() {
        this->variables = std::map<std::string, Var*>();
    }

    Var* getVariable(std::string name);

    void setVariable(std::string name);

    Var* getNumberAsVariable(int value);

    std::map<std::string, Var*> variables;
    int memory_pointer = 1;
};

#endif