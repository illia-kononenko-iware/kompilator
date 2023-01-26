#include <assert.h>
#include <algorithm>

#include <iostream>
#include "var_map.h"

void var_map::setCommandsVector(std::vector<Command*> *commands) {
    this->commands = commands;
}

void var_map::setCodeGeneratorCurrentProcedureName(std::string *name) {
    this->code_generator_current_procedure_name = name;
}

Var* var_map::getVariable(std::string name) {
    Var* var;

    // std::cout << "Name of currentProcedure " << this->getCurrentProcedure()->getName() << " and inside code_generator: " << *this->code_generator_current_procedure_name << std::endl;

    if ( *this->code_generator_current_procedure_name == "main" ) {
        var = this->variables[name];
    } else {
        var = this->procedures[ *this->code_generator_current_procedure_name ]->local_variables[name];

        if (var == 0) {

            ProcedureClass *procedure_instance = this->getProcedure( *this->code_generator_current_procedure_name );

            for (int i = 0; i < procedure_instance->parameters.size(); i++ ) {
                if (procedure_instance->parameters[i]->getName() == name) {
                    var = procedure_instance->parameters[i];
                    // std::cout << "Found parametr '" << var->getName() << "' with address " << var->getAddressAsString() << std::endl;
                }
            }
        }
    }

    if ( var == 0 ) {
        throw (std::string) "Variable '" + name + "' wasn't declared in this scope\n";
    } 

    if ( this->isCallingArguments() ) {
        // std::cout << "--------------- Calling arguments for " << this->getCurrentProcedure()->getName() << "--------------------\n";
    }

    // std::cout << "    var " << name << " is now in " << *this->code_generator_current_procedure_name << ". It is " << var->isInitialized() << " and has scope << " << var->getScope() << " >> \n";

    if ( var->getScope() != *this->code_generator_current_procedure_name ) {
        throw (std::string) "Variable '" + name + "' wasn't declared in this scope\n";
    }
    return var;
}

void var_map::setProcedureName() {
    std::string name = this->getCurrentProcedure()->getName();
    Var* new_varInstance = new Var(name, this->memory_pointer, false );
    this->variables[name] = new_varInstance;
    this->memory_pointer++;
    while (this->memory_pointer > 99990 && this->memory_pointer < 99999) {
        this->memory_pointer++;
    }
    this->getCurrentProcedure()->setJumpVariable(new_varInstance);
}

void var_map::setVariable(std::string name) {

    if ( this->procedures[name] != 0 ) {
        throw (std::string) "Cannot declare variable with name '" + name + "' as procedure with same name was already declared\n";
    }

    if (this->isCallingArguments()) {

        // std::cout << "Var name:" << name << std::endl;

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

        if ( this->isCreatingParameters() ) {
            // std::cout << "Creating param " << name << std::endl;
        } else {
            if ( this->isDeclaringProcedures() ) {
                // std::cout << "Creating new var " << name << " in procedure" << std::endl;
            } else {
                // std::cout << "Creating new var " << name << " in main" << std::endl;
            }
        }

        if ( this->isDeclaringProcedures() ) {
            for (int i = 0; i < this->getCurrentProcedure()->parameters.size(); i++ ) {
                // std::cout << name << " - " << this->getCurrentProcedure()->parameters[i]->getName() << std::endl;
                if (this->getCurrentProcedure()->parameters[i]->getName() == name) {
                    throw (std::string) "Parameter '" + name + "' declared second time in " + this->getCurrentProcedure()->getName() + " procedure\n";
                }
            }

            if ( this->getCurrentProcedure()->local_variables[name] != 0 ) {
                throw (std::string) "Variable '" + name + "' declared second time in " + this->getCurrentProcedure()->getName() + " procedure\n";
            }
        } else {
            if ( this->variables[name] != 0 && this->variables[name]->getScope() == "main" ) {
                throw (std::string) "Variable '" + name + "' declared second time in main\n";
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

        // std::cout << new_varInstance->getName() << " - " << new_varInstance->getAddress() << ", " << new_varInstance->isInitialized() << std::endl; 

        // std::cout << "Inside creating var. isCreating param: " << this->isCreatingParameters() << "\nisDeclaringProcedures: " << this->isDeclaringProcedures() << " for var " << this->variables[name]->getAddress() << " " << name << std::endl << std::endl;
        this->memory_pointer++;
        while (this->memory_pointer > 99990 && this->memory_pointer < 99999) {
            this->memory_pointer++;
        }

    }
}

void var_map::callProcedure(std::string name) {
    // std::cout << "- Procedure ( " << name << " )\n";
    if (!this->isCallingArguments()) {
        // std::cout << "-- Procedure creation ( " << name << " )\n";
        this->setProcedure(name);
    } else {
        // std::cout << "--- Call procedure ( " << name << " )\n";
    }

    this->setCurrentProcedure(name);
    // std::cout << this->getCurrentProcedure()->getAddress();
}

ProcedureClass* var_map::getProcedure(std::string name) {
    if ( this->procedures[name] == 0 ) {
        throw (std::string) "Call of procedure '" + name + "' that is not declared\n";
    }
    return this->procedures[name];
}

void var_map::setProcedure(std::string name) {
    if ( this->procedures[name] != 0 ) {
        throw (std::string) "Procedure '" + name + "' is already declared\n";
    }
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