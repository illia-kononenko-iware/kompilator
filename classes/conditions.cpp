#include "code_generator.h"

void code_generator::Conditions::addCommand(CMD name, string param = "") {
    if (param == "") {
        this->codeGen.commands.push_back( new Command( name ) );
    } else {
        this->codeGen.commands.push_back( new Command( name, param ) );
    }
}

void code_generator::Conditions::addPreparedCommand(Command *command) {
    this->codeGen.commands.push_back( command );
}

void code_generator::Conditions::checkVars(Var* var1, Var* var2) {
    if ( !var1->isInitialized() ) {
        throw (string) "Variable '" + var1->getName() + "' wasn't initialized\n";
    } 
    
    if ( !var2->isInitialized() ) {
        throw (string) "Variable '" + var2->getName() + "' wasn't initialized\n";
    }
}

void code_generator::Conditions::switchInsideConditionFlag() {
    this->codeGen.setInsideConditionFlag(true);
}

Condition code_generator::Conditions::greater(Var* var1, Var* var2) {

    this->switchInsideConditionFlag();
    
    this->checkVars(var1, var2);

    
    int beforeCondPtr = this->codeGen.commands.size();

    Command* falseJump = new Command(JZERO, "");

    string var2Address = var2->getAddressAsString();

    if ( var2->isConstant() ) {
        string constAddress = to_string( this->codeGen.getMemoryPointerForConst() );

        this->addCommand( SET, var2->getConstValueAsString() );

        this->addCommand( STORE, constAddress );
        
        var2Address = constAddress;
    } 
    
    if ( var1->isConstant() ) {
        this->addCommand( SET, var1->getConstValueAsString() );
    } else {
        this->addPreparedCommand( var1->getLoadCommand() );
    }

    if ( var2->isParameter() ) {
        this->addPreparedCommand( var2->getSubCommand() );
    } else {
        this->addCommand( SUB, var2Address );
    }

    this->addPreparedCommand( falseJump );

    int afterCondPtr = this->codeGen.commands.size();

    Condition condition_instance{
        .beforeCondPtr = beforeCondPtr,
        .afterCondPtr = afterCondPtr,
        .afterTrueJumpPtr = 0,
        .falseJump = falseJump
        };

    return condition_instance;
}

Condition code_generator::Conditions::greaterOrEqual(Var* var1, Var* var2) {
    
    this->switchInsideConditionFlag();
    
    this->checkVars(var1, var2);

    int beforeCondPtr = this->codeGen.commands.size();

    Command* falseJump = new Command(JPOS, "");

    string var1Address = var1->getAddressAsString();

     if ( var1->isConstant() ) {
        string constAddress = to_string( this->codeGen.getMemoryPointerForConst() );

        this->addCommand( SET, var1->getConstValueAsString() );

        this->addCommand( STORE, constAddress );
        
        var1Address = constAddress;
    } 

    if ( var2->isConstant() ) {
        this->addCommand( SET, var2->getConstValueAsString() );
    } else {
        this->addPreparedCommand( var2->getLoadCommand() );
    }

    if ( var1->isParameter() ) {
        this->addPreparedCommand( var1->getSubCommand() );
    } else {
        this->addCommand( SUB, var1Address );
    }

    this->addPreparedCommand( falseJump );

    int afterCondPtr = this->codeGen.commands.size();

    Condition condition_instance{
        .beforeCondPtr = beforeCondPtr,
        .afterCondPtr = afterCondPtr,
        .afterTrueJumpPtr = 0,
        .falseJump = falseJump
        };

    return condition_instance;
}

Condition code_generator::Conditions::less(Var* var1, Var* var2) {
    
    this->switchInsideConditionFlag();
    
    this->checkVars(var1, var2);

    int beforeCondPtr = this->codeGen.commands.size();

    Command* falseJump = new Command(JZERO, "");

    string var1Address = var1->getAddressAsString();

    if ( var1->isConstant() ) {
        
        string constAddress = to_string( this->codeGen.getMemoryPointerForConst() );

        this->addCommand( SET, var1->getConstValueAsString() );
        
        this->addCommand( STORE, constAddress );
        
        var1Address = constAddress;
    } 

    if ( var2->isConstant() ) {
        this->addCommand( SET, var2->getConstValueAsString() );
    } else {
        this->addPreparedCommand( var2->getLoadCommand() );
    }

    if ( var1->isParameter() ) {
        this->addPreparedCommand( var1->getSubCommand() );
    } else {
        this->addCommand( SUB, var1Address );
    }

    this->addPreparedCommand( falseJump );

    int afterCondPtr = this->codeGen.commands.size();

    Condition condition_instance{
        .beforeCondPtr = beforeCondPtr,
        .afterCondPtr = afterCondPtr,
        .afterTrueJumpPtr = 0,
        .falseJump = falseJump
        };

    return condition_instance;
}

Condition code_generator::Conditions::lessOrEqual(Var* var1, Var* var2) {
    
    this->switchInsideConditionFlag();

    this->checkVars(var1, var2);

    int beforeCondPtr = this->codeGen.commands.size();

    Command* falseJump = new Command(JPOS, "");

    string var2Address = var2->getAddressAsString();

    if ( var2->isConstant() ) {

        string constAddress = to_string( this->codeGen.getMemoryPointerForConst() );

        this->addCommand( SET, var2->getConstValueAsString() );
        this->addCommand( STORE, constAddress );        
        var2Address = constAddress;

    } 

    if ( var1->isConstant() ) {
        this->addCommand( SET, var1->getConstValueAsString() );
    } else {
        this->addPreparedCommand( var1->getLoadCommand() );
    }

    if ( var2->isParameter() ) {
        this->addPreparedCommand( var2->getSubCommand() );
    } else {
        this->addCommand( SUB, var2Address );
    }

    this->addPreparedCommand( falseJump );

    int afterCondPtr = this->codeGen.commands.size();

    Condition condition_instance{
        .beforeCondPtr = beforeCondPtr,
        .afterCondPtr = afterCondPtr,
        .afterTrueJumpPtr = 0,
        .falseJump = falseJump
        };

    return condition_instance;
}

Condition code_generator::Conditions::equal(Var* var1, Var* var2) {

    this->switchInsideConditionFlag();

    this->checkVars(var1, var2);

    string var1Address = var1->getAddressAsString();
    string var2Address = var2->getAddressAsString();

    int beforeCondPtr = this->codeGen.commands.size();

    Command* falseJump = new Command(JPOS, "");

    if ( var1->isConstant() ) {

        if ( var1->getConstValue() == 0 ) {
            falseJump = new Command( JPOS, "" );

            this->addPreparedCommand( var2->getLoadCommand() );
            this->addPreparedCommand( falseJump );

            int afterCondPtr = this->codeGen.commands.size();

            Condition condition_instance{
            .beforeCondPtr = beforeCondPtr,
            .afterCondPtr = afterCondPtr,
            .afterTrueJumpPtr = 0,
            .falseJump = falseJump
            };

            return condition_instance;
        }

        string constAddress = to_string( this->codeGen.getMemoryPointerForConst() );

        this->addCommand( SET, var1->getConstValueAsString() );
        this->addCommand( STORE, constAddress );        
        var1Address = constAddress;
        
    } 

    if ( var2->isConstant() ) {
            
        if ( var2->getConstValue() == 0 ) {
            falseJump = new Command( JPOS, "" );

            this->addPreparedCommand( var1->getLoadCommand() );
            this->addPreparedCommand( falseJump );

            int afterCondPtr = this->codeGen.commands.size();

            Condition condition_instance{
            .beforeCondPtr = beforeCondPtr,
            .afterCondPtr = afterCondPtr,
            .afterTrueJumpPtr = 0,
            .falseJump = falseJump
            };

            return condition_instance;
        }

        string constAddress = to_string( this->codeGen.getMemoryPointerForConst() + 1 );

        this->addCommand( SET, var2->getConstValueAsString() );
        this->addCommand( STORE, constAddress );        
        var2Address = constAddress;
        
    }

    if ( var1->isParameter() ) {
        this->addPreparedCommand( var1->getLoadCommand() );
    } else {
        this->addCommand( LOAD, var1Address );
    }

    if ( var2->isParameter() ) {
        this->addPreparedCommand( var2->getSubCommand() );
    } else {
        this->addCommand( SUB, var2Address );
    }

    this->addCommand( JPOS, to_string( this->codeGen.getK() + 3 ) );
    
    if ( var2->isParameter() ) {
        this->addPreparedCommand( var2->getLoadCommand() );
    } else {
        this->addCommand( LOAD, var2Address );
    }

    if ( var1->isParameter() ) {
        this->addPreparedCommand( var1->getSubCommand() );
    } else {
        this->addCommand( SUB, var1Address );
    }

    this->addPreparedCommand( falseJump );
    
    int afterCondPtr = this->codeGen.commands.size();

    Condition condition_instance{
        .beforeCondPtr = beforeCondPtr,
        .afterCondPtr = afterCondPtr,
        .afterTrueJumpPtr = 0,
        .falseJump = falseJump
        };

    return condition_instance;
}

Condition code_generator::Conditions::notEqual(Var* var1, Var* var2) {

    this->switchInsideConditionFlag();

    this->checkVars(var1, var2);

    Condition condition_instance = this->equal(var1, var2);
    condition_instance.falseJump->setParam(
        to_string( this->codeGen.getK() + 1)
    );

    condition_instance.falseJump = new Command(JUMP, "");
    this->codeGen.commands.push_back(condition_instance.falseJump);
    condition_instance.afterCondPtr++;
    return condition_instance;
}