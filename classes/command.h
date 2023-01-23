#ifndef COMMAND_H
#define COMMAND_H

#include <string>
#include <map>

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
    Command(CMD name, std::string param);
    void setParam(std::string param);

    std::string parseToString();

private:
    CMD name;
    std::string parameter;
    static std::map<CMD, std::string> CMD_to_str;
};

#endif