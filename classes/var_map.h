#ifndef VAR_MAP_H
#define VAR_MAP_H

#include <map>
#include <vector>
#include <string>
#include "variable.h"
#include "procedure.h"
#include "command.h"
using namespace std;

class var_map {
public:
    var_map() {
        this->variables = map<string, Var*>();
        this->procedures = map<string, ProcedureClass*>();
    }

    void callProcedure(string name);

    Var* getVariable(string name);

    void setVariable(string name);

    ProcedureClass* getProcedure(string name);

    void setProcedure(string name);

    void setProcedureName();

    void setCommandsVector(vector<Command*> *commands);

    Var* getNumberAsVariable(uint64_t value);

    void startCreatingParameters();
    void stopCreatingParameters();

    bool isCreatingParameters();

    /** Calling arguments in proc_head */

    void stopCreatingProcedures();

    void startCallingArguments();
    void stopCallingArguments();

    bool isCallingArguments();
    

    void startDeclaringProcedures();
    void stopDeclaringProcedures();
    bool isDeclaringProcedures();


    void setCurrentProcedure(string name);

    void setCodeGeneratorCurrentProcedureName(string *name);

    ProcedureClass* getCurrentProcedure();

    map<string, Var*> variables;
    map<string, ProcedureClass*> procedures;
    int memory_pointer = 1;
    int procedure_pointer = 1;
    bool isCreatingParametersFlag = true;
    bool isCallingArgumentsFlag = false;
    bool isDeclaringProceduresFlag = true;
    string *code_generator_current_procedure_name;

    private:
        vector<Command*> *commands;
        ProcedureClass *currentProcedure;

};

#endif