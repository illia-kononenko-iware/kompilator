#ifndef CODE_GENERATOR_H
#define CODE_GENERATOR_H

#include <string>
#include <map>
#include "var_map.h"
#include "command.h"
using namespace std;

struct Condition {
    int beforeCondPtr;
    int afterCondPtr;
    int afterTrueJumpPtr;
    Command* falseJump;
};

struct IfElseParam {
    int ptr;
    Command* jump;
};

class code_generator {
    public:
        code_generator(var_map* var_map_instance) : operations(*this), conditions(*this), flowControler(*this) {
            this->var_map_instance = var_map_instance;
            this->commands.push_back(new Command(JUMP, ""));
            this->var_map_instance->setCommandsVector(&this->commands);
            this->var_map_instance->setCodeGeneratorCurrentProcedureName(&this->currentProcedureName);
        };
        void readVariable(Var* variable);
        void writeVariable(Var* variable);

        string* loadVar(Var* variable);

        void assignValue(Var* variable);

        void endGenerateCode();
        string getCode();

        int getK();

        int getMemoryPointerForConst();

        void finishProceduresDeclaration(int k);

        void finishProcedureDeclaration();

        void startProcedureDeclaration();

        void finishProcedure();

        void jumpToStartProcedure();

        string currentProcedureName;

        void setInsideConditionFlag(bool flag);

        bool getInsideConditionFlag();

    class Operations {
    public:
        Operations(code_generator& code) : codeGen(code) {};

        void add(Var* var1, Var* var2);
        void sub(Var* var1, Var* var2);
        void mul(Var* var1, Var* var2);
        void div(Var* var1, Var* var2);
        void mod(Var* var1, Var* var2);
        void addCommand(CMD name, string param);
        void addPreparedCommand(Command *command);
        void checkVars(Var* var1, Var* var2);
        
    private:

        void mulOneConstant(Var* var, int const_value);
        void divideByConstant(Var* var, int const_value);
        void modByConstant(Var* var, int const_value);

        code_generator& codeGen;
    };

    class Conditions {
        public:
            Conditions(code_generator& code) : codeGen(code) {};
            Condition equal(Var* var1, Var* var2);
            Condition notEqual(Var* var1, Var* var2);
            Condition less(Var* var1, Var* var2);
            Condition lessOrEqual(Var* var1, Var* var2);
            Condition greater(Var* var1, Var* var2);
            Condition greaterOrEqual(Var* var1, Var* var2);
            void addCommand(CMD name, string param);
            void addPreparedCommand(Command *command);
            void checkVars(Var* var1, Var* var2);
            void switchInsideConditionFlag();
            
        private:
            code_generator& codeGen;
    };

    class FlowControler {
    public:
        FlowControler(code_generator& code) : codeGen(code) {};

        void singleIf(Condition cond);
        
        IfElseParam ifElseFirst(Condition cond);
        void ifElseSecond(IfElseParam param);

        void whileLoop(Condition cond);

        int repeatUntil_Start();
        void repeatUntil_End(Condition cond, int ptr);
        void addCommand(CMD name, string param);
        void addPreparedCommand(Command *command);

    private:
        code_generator& codeGen;
    };

    Operations* doOperation() { return &this->operations; }
    Conditions* makeCondition() { return &this->conditions; }
    FlowControler* flowControl() { return &this->flowControler; }

    private:
        var_map* var_map_instance;
        vector<Command*> commands;
        Operations operations;
        Conditions conditions;
        FlowControler flowControler;
        int memory_pointer_for_const = 99990;
        bool insideConditionFlag = false;
};

#endif