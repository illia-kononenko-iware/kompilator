#include <assert.h>
#include <algorithm>

#include <iostream>
#include "var_map.h"
using namespace std;

void var_map::setCommandsVector(vector<Command*> *commands) {
    this->commands = commands;
}

void var_map::setCodeGeneratorCurrentProcedureName(string *name) {
    this->code_generator_current_procedure_name = name;
}

Var* var_map::getVariable(string name) {
    Var* var;

    if ( *this->code_generator_current_procedure_name == "main" ) {
        var = this->variables[name];
    } else {
        var = this->procedures[ *this->code_generator_current_procedure_name ]->local_variables[name];

        if (var == 0) {

            ProcedureClass *procedure_instance = this->getProcedure( *this->code_generator_current_procedure_name );

            for (int i = 0; i < procedure_instance->parameters.size(); i++ ) {
                if (procedure_instance->parameters[i]->getName() == name) {
                    var = procedure_instance->parameters[i];
                }
            }
        }
    }

    if ( var == 0 ) {
        throw (string) "Variable '" + name + "' wasn't declared in this scope\n";
    } 

    if ( var->getScope() != *this->code_generator_current_procedure_name ) {
        throw (string) "Variable '" + name + "' wasn't declared in this scope\n";
    }
    return var;
}

void var_map::setProcedureName() {
    string name = this->getCurrentProcedure()->getName();
    Var* new_varInstance = new Var(name, this->memory_pointer, false );
    this->variables[name] = new_varInstance;
    this->memory_pointer++;
    while (this->memory_pointer > 99990 && this->memory_pointer < 99999) {
        this->memory_pointer++;
    }
    this->getCurrentProcedure()->setJumpVariable(new_varInstance);
}

void var_map::setVariable(string name) {

    if ( this->procedures[name] != 0 ) {
        throw (string) "Cannot declare variable with name '" + name + "' as procedure with same name was already declared\n";
    }

    if (this->isCallingArguments()) {

        Var* var = this->getVariable(name);

        var->initialize();

        if (var->isParameter()) {
            this->commands->push_back(new Command( LOAD, var->getAddressAsString() ) );
        } else {
            this->commands->push_back(new Command( SET, var->getAddressAsString() ) );
        }

        int currentParamIndex = this->getCurrentProcedure()->getCurrentParameterIndex();
        this->commands->push_back(new Command( STORE, this->getCurrentProcedure()->parameters[currentParamIndex]->getAddressAsString() ) );
        this->getCurrentProcedure()->incrementCurrentParameterIndex();

    } else {

        if ( this->isDeclaringProcedures() ) {
            for (int i = 0; i < this->getCurrentProcedure()->parameters.size(); i++ ) {
                if (this->getCurrentProcedure()->parameters[i]->getName() == name) {
                    throw (string) "Parameter '" + name + "' declared second time in " + this->getCurrentProcedure()->getName() + " procedure\n";
                }
            }

            if ( this->getCurrentProcedure()->local_variables[name] != 0 ) {
                throw (string) "Variable '" + name + "' declared second time in " + this->getCurrentProcedure()->getName() + " procedure\n";
            }
        } else {
            if ( this->variables[name] != 0 && this->variables[name]->getScope() == "main" ) {
                throw (string) "Variable '" + name + "' declared second time in main\n";
            }
        }

        

        Var* new_varInstance = new Var(name, this->memory_pointer, this->isCreatingParameters() );

        if ( this->isDeclaringProcedures() ) {

            new_varInstance->setScope( this->getCurrentProcedure()->getName() );

            if ( this->isCreatingParameters() ) {
                new_varInstance->initialize();
                this->getCurrentProcedure()->parameters.push_back(new_varInstance);
            } else {
                this->getCurrentProcedure()->local_variables[name] = new_varInstance;
            }

        } else {
            
            this->variables[name] = new_varInstance;
            new_varInstance->setScope( "main" );

        }


        this->memory_pointer++;
        while (this->memory_pointer > 99990 && this->memory_pointer < 99999) {
            this->memory_pointer++;
        }

    }
}

void var_map::callProcedure(string name) {
    if (!this->isCallingArguments()) {
        this->setProcedure(name);
    } else {
    }

    this->setCurrentProcedure(name);
}

ProcedureClass* var_map::getProcedure(string name) {
    if ( this->procedures[name] == 0 ) {
        throw (string) "Call of procedure '" + name + "' that is not declared\n";
    }
    return this->procedures[name];
}

void var_map::setProcedure(string name) {
    if ( this->procedures[name] != 0 ) {
        throw (string) "Procedure '" + name + "' is already declared\n";
    }
    this->procedures[name] = new ProcedureClass( name, this->commands->size()  );
}

void var_map::setCurrentProcedure(string name) {
    ProcedureClass* procedure = this->getProcedure(name);
    this->currentProcedure = procedure;
}

ProcedureClass* var_map::getCurrentProcedure() {
    return this->currentProcedure;
}

Var* var_map::getNumberAsVariable(uint64_t value) {
    Var* return_Var = new Var(value);
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


void var_map::startCallingArguments() {
    this->isCallingArgumentsFlag = true;
}

void var_map::stopCallingArguments() {
    this->isCallingArgumentsFlag = false;
}

bool var_map::isCallingArguments() {
    return this->isCallingArgumentsFlag;
}


void var_map::startDeclaringProcedures() {
    this->isDeclaringProceduresFlag = true;
}

void var_map::stopDeclaringProcedures() {
    this->isDeclaringProceduresFlag = false;
}

bool var_map::isDeclaringProcedures() {
    return this->isDeclaringProceduresFlag;
}