#include "code_generator.h"

// finish all constants and variables from both sides

Condition code_generator::Conditions::greater(Var* var1, Var* var2) {
    
    int beforeCondPtr = this->codeGen.commands.size();

    Command* falseJump = new Command(JZERO, "");

    this->codeGen.commands.push_back(new Command(LOAD, std::to_string(var1->getAddress())));
    this->codeGen.commands.push_back(new Command(SUB, std::to_string(var2->getAddress())));
    this->codeGen.commands.push_back(falseJump);

    int afterCondPtr = this->codeGen.commands.size();

    // std::cout << "Before condition pointer: " << beforeCondPtr << std::endl;

    Condition condition_instance{
        .beforeCondPtr = beforeCondPtr,
        .afterCondPtr = afterCondPtr,
        .afterTrueJumpPtr = 0,
        .falseJump = falseJump
        };

    // std::cout << "Before condition pointer inside condition struct: " << condition_instance.beforeCondPtr << std::endl;

    return condition_instance;
}

Condition code_generator::Conditions::greaterOrEqual(Var* var1, Var* var2) {
    
    int beforeCondPtr = this->codeGen.commands.size();

    Command* falseJump = new Command(JPOS, "");

    this->codeGen.commands.push_back(new Command(LOAD, std::to_string(var2->getAddress())));
    this->codeGen.commands.push_back(new Command(SUB, std::to_string(var1->getAddress())));
    this->codeGen.commands.push_back(falseJump);

    int afterCondPtr = this->codeGen.commands.size();

    // std::cout << "Before condition pointer: " << beforeCondPtr << std::endl;


    Condition condition_instance{
        .beforeCondPtr = beforeCondPtr,
        .afterCondPtr = afterCondPtr,
        .afterTrueJumpPtr = 0,
        .falseJump = falseJump
        };

    // std::cout << "Before condition pointer inside condition struct: " << condition_instance.beforeCondPtr << std::endl;

    return condition_instance;
}

Condition code_generator::Conditions::less(Var* var1, Var* var2) {
    
    int beforeCondPtr = this->codeGen.commands.size();

    Command* falseJump = new Command(JZERO, "");

    this->codeGen.commands.push_back(new Command(LOAD, std::to_string(var2->getAddress())));
    this->codeGen.commands.push_back(new Command(SUB, std::to_string(var1->getAddress())));
    this->codeGen.commands.push_back(falseJump);

    int afterCondPtr = this->codeGen.commands.size();

    // std::cout << "Before condition pointer: " << beforeCondPtr << std::endl;

    Condition condition_instance{
        .beforeCondPtr = beforeCondPtr,
        .afterCondPtr = afterCondPtr,
        .afterTrueJumpPtr = 0,
        .falseJump = falseJump
        };

    // std::cout << "Before condition pointer inside condition struct: " << condition_instance.beforeCondPtr << std::endl;

    return condition_instance;
}

Condition code_generator::Conditions::lessOrEqual(Var* var1, Var* var2) {
    
    int beforeCondPtr = this->codeGen.commands.size();

    Command* falseJump = new Command(JPOS, "");

    this->codeGen.commands.push_back(new Command(LOAD, std::to_string(var1->getAddress())));
    this->codeGen.commands.push_back(new Command(SUB, std::to_string(var2->getAddress())));
    this->codeGen.commands.push_back(falseJump);

    int afterCondPtr = this->codeGen.commands.size();

    // std::cout << "Before condition pointer: " << beforeCondPtr << std::endl;


    Condition condition_instance{
        .beforeCondPtr = beforeCondPtr,
        .afterCondPtr = afterCondPtr,
        .afterTrueJumpPtr = 0,
        .falseJump = falseJump
        };

    // std::cout << "Before condition pointer inside condition struct: " << condition_instance.beforeCondPtr << std::endl;

    return condition_instance;
}

Condition code_generator::Conditions::equal(Var* var1, Var* var2) {

    int beforeCondPtr = this->codeGen.commands.size();

    Command* firstCheckFalseJump = new Command(JPOS, "");

    Command* falseJump;

    int var1_value;
    int var2_value;

    if (var2->isConstant()) {
        var1_value = var1->getConstValue();
        if (var1_value == 0) {
            falseJump = new Command(JZERO, "");
            this->codeGen.commands.push_back(new Command(LOAD, std::to_string(var1->getAddress())));
            this->codeGen.commands.push_back(falseJump);
        }
    } else {
        falseJump = new Command(JPOS, "");

        // jesli X - Y > 0 czyli X > Y( != 0 ) to mamy falseJump i nie dzialamy (dalej nie sprawdzamy i po prostu skoczemy do warunku falseJump)
        this->codeGen.commands.push_back(new Command(LOAD, std::to_string(var1->getAddress())));
        this->codeGen.commands.push_back(new Command(SUB, std::to_string(var2->getAddress())));
        
        firstCheckFalseJump->setParam(std::to_string(this->codeGen.commands.size() + 3));

        this->codeGen.commands.push_back(firstCheckFalseJump);
        // teraz x nie moze byc wiekszy od y bo nie przepuscilo by dalej w poprzednim warunku
        // jesli Y - X > 0 czyli Y > X ( != 0 ) to mamy falseJump i nie dzialamy bo Y okazal sie wiekszy za X
        this->codeGen.commands.push_back(new Command(LOAD, std::to_string(var2->getAddress())));
        this->codeGen.commands.push_back(new Command(SUB, std::to_string(var1->getAddress())));
        this->codeGen.commands.push_back(falseJump);
    }

    int afterCondPtr = this->codeGen.commands.size();

    // std::cout << "Before condition pointer: " << beforeCondPtr << std::endl;

    Condition condition_instance{
        .beforeCondPtr = beforeCondPtr,
        .afterCondPtr = afterCondPtr,
        .afterTrueJumpPtr = 0,
        .falseJump = falseJump
        };

    // std::cout << "Before condition pointer inside condition struct: " << condition_instance.beforeCondPtr << std::endl;

    return condition_instance;
}

Condition code_generator::Conditions::notEqual(Var* var1, Var* var2) {

    Condition condition_instance = this->equal(var1, var2);
    condition_instance.falseJump->setParam(
        std::to_string(this->codeGen.commands.size() + 1)
    );

    condition_instance.falseJump = new Command(JUMP, "");
    this->codeGen.commands.push_back(condition_instance.falseJump);
    condition_instance.afterCondPtr++;
    return condition_instance;
}