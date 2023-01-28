#include "code_generator.h"
#include <iostream>

void code_generator::setInsideConditionFlag(bool flag) {
    this->insideConditionFlag = flag;
}

bool code_generator::getInsideConditionFlag() {
    return this->insideConditionFlag;
}

void code_generator::readVariable(Var* variable) {
    this->commands.push_back(new Command(GET, to_string(variable->getAddress())));
    variable->initialize();
}

void code_generator::writeVariable(Var* variable) {
    if (variable->isConstant()) {
        this->commands.push_back(new Command(SET, to_string(variable->getConstValue())));
        this->commands.push_back(new Command(PUT, "0"));
    } else {
        if (variable->isParameter()) {
            this->commands.push_back(new Command(LOADI, to_string(variable->getAddress())));
            this->commands.push_back(new Command(PUT, "0" ));
        } else {
            if ( !variable->isInitialized() ) {
                throw (string) "Variable '" + variable->getName() + "' wasn't initialized\n";
            }
            this->commands.push_back(new Command(PUT, to_string(variable->getAddress())));
        }
    }
}

string* code_generator::loadVar(Var* variable) {
    if (variable->isConstant()) {
        this->commands.push_back(new Command(SET, to_string(variable->getConstValue())));
        return new string("0");
    } else {
        
        if (variable->isParameter()) {
            this->commands.push_back(new Command(LOADI, to_string(variable->getAddress())));
        } else {
            this->commands.push_back(new Command(LOAD, to_string(variable->getAddress())));
        }

        return new string(to_string(variable->getAddress()));
    }
}

void code_generator::assignValue(Var* variable) {
    if (variable->isParameter()) {
        this->commands.push_back(new Command(STOREI, to_string(variable->getAddress())));
    } else {
        variable->initialize();
        this->commands.push_back(new Command(STORE, to_string(variable->getAddress())));
    }
}

void code_generator::endGenerateCode() {
    this->commands.push_back(new Command(HALT));
}

void code_generator::finishProceduresDeclaration(int k) {
    if (k == 1) {
        this->commands.erase(this->commands.begin());
    } else {
        this->commands[0]->setParam( to_string(k) );
    }
    this->var_map_instance->stopDeclaringProcedures();
    this->currentProcedureName = "main";
    this->var_map_instance->callProcedure("main");
}

string code_generator::getCode() {
    string code;
    for (Command* command : this->commands) {
        code.append(command->parseToString() + "\n" );
    }
    return code;
}

int code_generator::getK() {
    return this->commands.size();
}


int code_generator::getMemoryPointerForConst() {
    return this->memory_pointer_for_const;
}

void code_generator::finishProcedureDeclaration() {
    var_map_instance->startCreatingParameters();
    var_map_instance->startCallingArguments();
    this->currentProcedureName = var_map_instance->getCurrentProcedure()->getName();
    this->var_map_instance->setProcedureName();
}

void code_generator::finishProcedure() {
    var_map_instance->stopCallingArguments();
    this->commands.push_back(new Command(JUMPI, this->var_map_instance->getCurrentProcedure()->getJumpVariable()->getAddressAsString()  ));
}

void code_generator::jumpToStartProcedure() {
    string previousProcedureName = this->currentProcedureName;
    string jumpToProcedure = this->var_map_instance->getCurrentProcedure()->getAddressAsString();
    string storeToProcedure = this->var_map_instance->getCurrentProcedure()->getJumpVariable()->getAddressAsString();
    if (this->currentProcedureName != this->var_map_instance->getCurrentProcedure()->getName()) {
        this->var_map_instance->setCurrentProcedure(this->currentProcedureName);
    }

    this->commands.push_back(new Command(SET, to_string(this->getK() + 3 ) ));
    this->commands.push_back(new Command(STORE, storeToProcedure ));
    this->commands.push_back(new Command(JUMP, jumpToProcedure ));

    this->var_map_instance->setCurrentProcedure(previousProcedureName);
    this->currentProcedureName = previousProcedureName;
}