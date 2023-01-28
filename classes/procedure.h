#ifndef PROCEDURE_H
#define PROCEDURE_H

#include <string>
#include <iostream>
using namespace std;

class ProcedureClass {
    public:

        ProcedureClass(string name, int startAddress) {
            this->parameters = vector<Var*>();
            this->local_variables = map<string, Var*>();

            this->name = name;
            this->startAddress = startAddress;
        }

        Var* getJumpVariable() { return this->jumpVariable; }
        void setJumpVariable(Var* var) { this->jumpVariable = var; }

        int getAddress() { return this->startAddress; }

        string getAddressAsString() { return to_string(this->startAddress); }

        string getName() { return this->name; }

        vector<Var*> parameters;
        map<string, Var*> local_variables;

        int getCurrentParameterIndex() { return this->currentParameterIndex; }
        void incrementCurrentParameterIndex() { this->currentParameterIndex = (this->currentParameterIndex + 1) % this->parameters.size(); }

    private:
        string name;
        int startAddress;
        int currentParameterIndex = 0;
        Var* jumpVariable;
};

#endif