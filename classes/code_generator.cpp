#include "code_generator.h"
#include <iostream>

void code_generator::readVariable(Var* variable) {
    this->commands.push_back(new Command(GET, std::to_string(variable->getAddress())));
}

void code_generator::writeVariable(Var* variable) {
    // std::cout << "Inside write var " << variable->isConstant() << std::endl;
    if (variable->isConstant()) {
        // std::cout << "Inside write var " << variable->getConstValue() << std::endl;
        this->commands.push_back(new Command(SET, std::to_string(variable->getConstValue())));
        this->commands.push_back(new Command(PUT, "0"));
    } else {
        this->commands.push_back(new Command(PUT, std::to_string(variable->getAddress())));
    }
}

std::string* code_generator::loadVar(Var* variable) {
    // std::cout << "Inside load var " << variable->isConstant() << std::endl;
    if (variable->isConstant()) {
        // std::cout << "\nvariable->getConstValue() = " << variable->getConstValue() << std::endl;
        this->commands.push_back(new Command(SET, std::to_string(variable->getConstValue())));
        return new std::string("0");
    } else {
        this->commands.push_back(new Command(LOAD, std::to_string(variable->getAddress())));
        return new std::string(std::to_string(variable->getAddress()));
    }
}

void code_generator::assignValue(Var* variable) {
    this->commands.push_back(new Command(STORE, std::to_string(variable->getAddress())));
}

void code_generator::endGenerateCode() {
    this->commands.push_back(new Command(HALT));
}

std::string code_generator::getCode() {
    std::string code;
    // for (Command* command : this->commands) {
        // code.append(command->parseToString() + "\n" );
    // }
    for (int i = 0; i < this->commands.size(); i++) {
        code.append(std::to_string(i) + "  " + this->commands[i]->parseToString() + "\n" );
    }
    return code;
}

int code_generator::getK() {
    return this->commands.size();
}


int code_generator::getLatestMemoryPoint() {
    return this->var_map_instance->memory_pointer;
}

