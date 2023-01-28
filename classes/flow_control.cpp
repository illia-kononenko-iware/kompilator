#include "code_generator.h"

void code_generator::FlowControler::singleIf(Condition cond) {

    this->codeGen.setInsideConditionFlag(false);

    int jumpValue = this->codeGen.commands.size();
    cond.falseJump->setParam(to_string(jumpValue));

}

IfElseParam code_generator::FlowControler::ifElseFirst(Condition cond) {

    this->codeGen.setInsideConditionFlag(false);

    Command* jump = new Command(JUMP, "");
    this->codeGen.commands.push_back(jump);
    this->singleIf(cond);

    int ptr = this->codeGen.commands.size();
    return IfElseParam{.ptr = ptr, .jump = jump};
}

void code_generator::FlowControler::ifElseSecond(IfElseParam param) {
    this->codeGen.setInsideConditionFlag(false);
    int jumpValue = this->codeGen.commands.size();
    param.jump->setParam(to_string(jumpValue));
}

int code_generator::FlowControler::repeatUntil_Start() {
    return this->codeGen.commands.size();
}

void code_generator::FlowControler::repeatUntil_End(Condition condition, int k) {
    this->codeGen.setInsideConditionFlag(false);
    condition.falseJump->setParam(to_string(k));
}

void code_generator::FlowControler::whileLoop(Condition cond) {
    this->codeGen.setInsideConditionFlag(false);

    this->codeGen.commands.push_back(new Command(JUMP, to_string(cond.beforeCondPtr)));

    cond.falseJump->setParam(to_string( this->codeGen.commands.size() ));
}