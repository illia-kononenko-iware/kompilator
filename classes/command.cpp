#include "command.h"
#include <map>
#include <iostream>
using namespace std;

map<CMD, string> Command::CMD_to_str = {
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

Command::Command(CMD name, string param) {
    this->name = name;
    this->parameter = param;
}

void Command::setParam(string param) {
    this->parameter = param;
}

string Command::parseToString() {
    return Command::CMD_to_str[this->name] + " " + this->parameter;
}