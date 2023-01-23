#ifndef CODE_GENERATOR_H
#define CODE_GENERATOR_H

#include <string>
#include <map>
#include "var_map.h"
#include "command.h"

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

struct ProceduresParam {
    int startMainPtr;
};

class code_generator {
    public:
        code_generator(var_map* var_map_instance) : operations(*this), conditions(*this), flowControler(*this) {
            this->var_map_instance = var_map_instance;
        };
        void readVariable(Var* variable);
        void writeVariable(Var* variable);

        std::string* loadVar(Var* variable);

        void assignValue(Var* variable);

        void endGenerateCode();
        std::string getCode();

        int getK();

        int getLatestMemoryPoint();

    class Operations {
    public:
        Operations(code_generator& code) : codeGen(code) {};
        // std::string* add(Var* var1, Var* var2);
        // std::string* sub(Var* var1, Var* var2);
        // std::string* mul(Var* var1, Var* var2);
        // std::string* div(Var* var1, Var* var2);
        // std::string* mod(Var* var1, Var* var2);

        void add(Var* var1, Var* var2);
        void sub(Var* var1, Var* var2);
        void mul(Var* var1, Var* var2);
        void div(Var* var1, Var* var2);
        void mod(Var* var1, Var* var2);

    private:
        // std::string* mulOneConstant(Constant* constant, Var* var);
        // std::string* divideByConstant(Constant* constant, Var* var);

        void mulOneConstant(Var* var, int const_value);
        void divideByConstant(Var* var, int const_value);

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

    private:
        code_generator& codeGen;
    };

    Operations* doOperation() { return &this->operations; }
    Conditions* makeCondition() { return &this->conditions; }
    FlowControler* flowControl() { return &this->flowControler; }

    private:
        var_map* var_map_instance;
        std::vector<Command*> commands;
        Operations operations;
        Conditions conditions;
        FlowControler flowControler;
};

#endif