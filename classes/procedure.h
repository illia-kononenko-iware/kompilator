#ifndef PROCEDURE_H
#define PROCEDURE_H

#include <string>
#include <iostream>

class ProcedureClass {
    public:

        ProcedureClass(std::string name, int startAddress) {
            this->parameters = std::vector<Var*>();
            this->local_variables = std::map<std::string, Var*>();

            // std::cout << "Inside variable constructor\n";
            this->name = name;
            this->startAddress = startAddress;
        }

        Var* getJumpVariable() { return this->jumpVariable; }
        void setJumpVariable(Var* var) { this->jumpVariable = var; }

        int getAddress() { return this->startAddress; }

        std::string getAddressAsString() { return std::to_string(this->startAddress); }

        std::string getName() { return this->name; }

        std::vector<Var*> parameters;
        std::map<std::string, Var*> local_variables;

        int getCurrentParameterIndex() { return this->currentParameterIndex; }
        void incrementCurrentParameterIndex() { this->currentParameterIndex = (this->currentParameterIndex + 1) % this->parameters.size(); }

    private:
        std::string name;
        int startAddress;
        int currentParameterIndex = 0;
        Var* jumpVariable;
};

#endif