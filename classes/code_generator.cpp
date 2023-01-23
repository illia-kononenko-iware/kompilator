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
    if (variable->isConstant()) {
        // std::cout << "\nvariable->getConstValue() = " << variable->getConstValue() << std::endl;
        this->commands.push_back(new Command(SET, std::to_string(variable->getConstValue())));
        return new std::string("0");
    } else {
        std::cout << "Inside load var " << variable->getAddress() << " - " << variable->isParameter() << std::endl;
        
        if (variable->isParameter()) {
            this->commands.push_back(new Command(LOADI, std::to_string(variable->getAddress())));
        } else {
            this->commands.push_back(new Command(LOAD, std::to_string(variable->getAddress())));
        }

        return new std::string(std::to_string(variable->getAddress()));
    }
}

void code_generator::assignValue(Var* variable) {
    if (variable->isParameter()) {
        this->commands.push_back(new Command(STOREI, std::to_string(variable->getAddress())));
    } else {
        this->commands.push_back(new Command(STORE, std::to_string(variable->getAddress())));
    }
}

void code_generator::endGenerateCode() {
    this->commands.push_back(new Command(HALT));
}

void code_generator::finishProceduresDeclaration(int k) {
    if (k == 1) {
        this->commands.erase(this->commands.begin());
    } else {
        this->commands[0]->setParam( std::to_string(k) );
    }
    this->var_map_instance->stopDeclaringProcedures();
}

std::string code_generator::getCode() {
    std::string code;
    for (Command* command : this->commands) {
        code.append(command->parseToString() + "\n" );
    }
    // for (int i = 0; i < this->commands.size(); i++) {
        // code.append(std::to_string(i) + "  " + this->commands[i]->parseToString() + "\n" );
    // }
    return code;
}

int code_generator::getK() {
    return this->commands.size();
}


int code_generator::getLatestMemoryPoint() {
    return this->var_map_instance->memory_pointer;
}

ProcedureStruct code_generator::declareProcedure(std::string name) {
    std::cout << name << std::endl;
    // Procedure result_procedure;
    // result_procedure.startMainPtr = this->getK();
    // return result_procedure;
}

void code_generator::finishProcedureDeclaration() {
    std::cout << "After declarations\n"; 
    var_map_instance->startCreatingParameters();
    var_map_instance->startCallingArguments();
    this->currentProcedureName = var_map_instance->getCurrentProcedure()->getName();
    this->var_map_instance->setProcedureName();
    // var_map_instance->setVariable("jump to procedure");
}

void code_generator::startProcedureDeclaration() {
    std::cout << "Declarations\n"; 
    var_map_instance->stopCreatingParameters();
}

void code_generator::finishProcedure() {
    var_map_instance->stopCallingArguments();
    this->commands.push_back(new Command(JUMPI, this->var_map_instance->getCurrentProcedure()->getJumpVariable()->getAddressAsString()  ));
}

ProcedureStruct code_generator::getProcedure() {
    ProcedureStruct result_procedure;
    return result_procedure;
}

void code_generator::jumpToStartProcedure() {
    // if (this->currentProcedureName != this->var_map_instance->getCurrentProcedure()->getName()) {
    //     this->var_map_instance->setCurrentProcedure(this->currentProcedureName);
    // }
    this->commands.push_back(new Command(SET, std::to_string(this->getK() + 3 ) ));
    this->commands.push_back(new Command(STORE, this->var_map_instance->getCurrentProcedure()->getJumpVariable()->getAddressAsString()  ));
    this->commands.push_back(new Command(JUMP, this->var_map_instance->getCurrentProcedure()->getAddressAsString()  ));
}