#include "command.h"
#include <map>
#include <iostream>

std::map<CMD, std::string> Command::CMD_to_str = {
    { GET, "GET" },
    { PUT, "PUT" },
    { LOAD, "LOAD" },
    { STORE, "STORE" },
    { LOADI, "LOADI" },
    { STOREI, "STOREI" },
    { ADD, "ADD" },
    { SUB, "SUB" },
    { ADDI, "ADDI" },
    { SUBI, "SUBI" },
    { SET, "SET" },
    { HALF, "HALF" },
    { JUMP, "JUMP" },
    { JPOS, "JPOS" },
    { JZERO, "JZERO" },
    { JUMPI, "JUMPI" },
    { HALT, "HALT" }
};

Command::Command(CMD name) {
    this->name = name;
}

Command::Command(CMD name, std::string param) {
    this->name = name;
    this->parameter = param;
}

void Command::setParam(std::string param) {
    this->parameter = param;
}

std::string Command::parseToString() {
    return Command::CMD_to_str[this->name] + " " + this->parameter;
}