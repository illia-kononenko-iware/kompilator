#ifndef COMMAND_H
#define COMMAND_H

#include <string>
#include <map>
using namespace std;

enum CMD {
    GET,
    PUT,
    LOAD,
    STORE,
    LOADI,
    STOREI,
    ADD,
    SUB,
    ADDI,
    SUBI,
    SET,
    HALF,
    JUMP,
    JPOS,
    JZERO,
    JUMPI,
    HALT
};

class Command {
public:
    Command(CMD name);
    Command(CMD name, string param);
    void setParam(string param);

    string parseToString();

private:
    CMD name;
    string parameter;
    static map<CMD, string> CMD_to_str;
};

#endif