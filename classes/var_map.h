#ifndef VAR_MAP_H
#define VAR_MAP_H

#include <map>
#include <vector>
#include <string>
#include "variable.h"
#include "procedure.h"
#include "command.h"

class var_map {
public:
    var_map() {
        this->variables = std::map<std::string, Var*>();
        this->procedures = std::map<std::string, ProcedureClass*>();
    }

    void callProcedure(std::string name);

    Var* getVariable(std::string name);

    void setVariable(std::string name);

    ProcedureClass* getProcedure(std::string name);

    void setProcedure(std::string name);

    void setProcedureName();

    void setCommandsVector(std::vector<Command*> *commands);

    Var* getNumberAsVariable(uint64_t value);

    void startCreatingParameters();
    void stopCreatingParameters();

    bool isCreatingParameters();

    /** Calling arguments in proc_head */

    void stopCreatingProcedures();

    void startCallingArguments();
    void stopCallingArguments();

    bool isCallingArguments();
    
    /** declaring procedures */

    void startDeclaringProcedures();
    void stopDeclaringProcedures();
    bool isDeclaringProcedures();

    /** */

    void setCurrentProcedure(std::string name);

    void setCodeGeneratorCurrentProcedureName(std::string *name);

    ProcedureClass* getCurrentProcedure();

    std::map<std::string, Var*> variables;
    std::map<std::string, ProcedureClass*> procedures;
    int memory_pointer = 1;
    int procedure_pointer = 1;
    bool isCreatingParametersFlag = true;
    bool isCallingArgumentsFlag = false;
    bool isDeclaringProceduresFlag = true;
    std::string *code_generator_current_procedure_name;

    private:
        std::vector<Command*> *commands;
        ProcedureClass *currentProcedure;

};

#endif