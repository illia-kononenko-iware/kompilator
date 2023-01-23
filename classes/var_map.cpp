#include <assert.h>
#include <algorithm>

#include <iostream>
#include "var_map.h"

void var_map::setCommandsVector(std::vector<Command*> *commands) {
    this->commands = commands;
}

Var* var_map::getVariable(std::string name) {
    return this->variables[name];
}

void var_map::setProcedureName() {
    std::string name = this->getCurrentProcedure()->getName();
    Var* new_varInstance = new Var(name, this->memory_pointer, false );
    this->variables[name] = new_varInstance;
    this->memory_pointer++;
    this->getCurrentProcedure()->setJumpVariable(new_varInstance);
}

void var_map::setVariable(std::string name) {
    if (this->isCallingArguments()) {

        std::cout << "Var name:" << name << std::endl;

        Var* var = this->getVariable(name);
        this->commands->push_back(new Command( SET, var->getAddressAsString() ) );

        int currentParamIndex = this->getCurrentProcedure()->getCurrentParameterIndex();

        this->commands->push_back(new Command( STORE, this->getCurrentProcedure()->parameters[currentParamIndex]->getAddressAsString() ) );

        this->getCurrentProcedure()->incrementCurrentParameterIndex();

    } else {

        Var* new_varInstance = new Var(name, this->memory_pointer, this->isCreatingParameters() );
        this->variables[name] = new_varInstance;

        if ( this->isDeclaringProcedures() ) {

            if ( this->isCreatingParameters() ) {
                this->getCurrentProcedure()->parameters.push_back(new_varInstance);
            } else {
                this->getCurrentProcedure()->local_variables[name] = new_varInstance;
            }

        }

        // std::cout << "Inside creating var. isCreating param: " << this->isCreatingParameters() << "\nisDeclaringProcedures: " << this->isDeclaringProcedures() << " for var " << this->variables[name]->getAddress() << " " << name << std::endl << std::endl;
        this->memory_pointer++;

    }
}

void var_map::callProcedure(std::string name) {
    if (!this->isCallingArguments()) {
        this->setProcedure(name);
    }
    this->setCurrentProcedure(name);
}

ProcedureClass* var_map::getProcedure(std::string name) {
    return this->procedures[name];
}

void var_map::setProcedure(std::string name) {
    this->procedures[name] = new ProcedureClass( name, this->commands->size()  );
}

void var_map::setCurrentProcedure(std::string name) {
    // std::cout << "Current procedure: " << name << std::endl;
    ProcedureClass* procedure = this->getProcedure(name);
    // std::cout << "Procedure name: " << procedure->getJumpVariable() << std::endl;
    this->currentProcedure = procedure;
}

ProcedureClass* var_map::getCurrentProcedure() {
    return this->currentProcedure;
}

Var* var_map::getNumberAsVariable(uint64_t value) {
    Var* return_Var = new Var(value);
    // std::cout << "Get number as variable 3\n"; 
    // std::cout << value << std::endl;
    // std::cout << return_Var->getConstValue() << std::endl;
    return return_Var;
}

void var_map::startCreatingParameters() {
    this->isCreatingParametersFlag = true;
}

void var_map::stopCreatingParameters() {
    this->isCreatingParametersFlag = false;
}

bool var_map::isCreatingParameters() {
    return this->isCreatingParametersFlag;
}

/** Calling arguments in proc_head */

void var_map::startCallingArguments() {
    this->isCallingArgumentsFlag = true;
}

void var_map::stopCallingArguments() {
    this->isCallingArgumentsFlag = false;
}

bool var_map::isCallingArguments() {
    return this->isCallingArgumentsFlag;
}

/** declaring procedures */

void var_map::startDeclaringProcedures() {
    this->isDeclaringProceduresFlag = true;
}

void var_map::stopDeclaringProcedures() {
    this->isDeclaringProceduresFlag = false;
}

bool var_map::isDeclaringProcedures() {
    return this->isDeclaringProceduresFlag;
}