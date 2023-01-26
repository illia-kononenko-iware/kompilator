#include "code_generator.h"
#include <iostream>

// finish for consts from both sides

void code_generator::Operations::addCommand(CMD name, std::string param = "") {
    if (param == "") {
        this->codeGen.commands.push_back( new Command( name ) );
    } else {
        this->codeGen.commands.push_back( new Command( name, param ) );
    }
}

void code_generator::Operations::addPreparedCommand(Command *command) {
    this->codeGen.commands.push_back( command );
}

void code_generator::Operations::checkVars(Var* var1, Var* var2) {
    // std::cout << "Inside check vars " << var1->isInitialized() << std::endl;
    if ( this->codeGen.getInsideConditionFlag() == false ) {
        if ( !var1->isInitialized() ) {
            throw (std::string) "Variable '" + var1->getName() + "' wasn't initialized\n";
        } 
        
        if ( !var2->isInitialized() ) {
            throw (std::string) "Variable '" + var2->getName() + "' wasn't initialized\n";
        }
    }
}

void code_generator::Operations::add(Var* var1, Var* var2) {

    this->checkVars(var1, var2);

    // var1 is number
    if (var1->isConstant()) {
        // and var2 is number
        if (var2->isConstant()) {
            int result = var1->getConstValue() + var2->getConstValue();

            this->addCommand(SET, std::to_string(result));

            return;
        }

        // var1 is number and var2 is var

        this->addCommand(SET, var1->getConstValueAsString() );
        
        this->addPreparedCommand( var2->getAddCommand() );

        return;
    }

    // var1 is var and var2 is number
    if (!var1->isConstant() && var2->isConstant()) {

        this->addCommand( SET, var2->getConstValueAsString() );

        this->addPreparedCommand( var1->getAddCommand() );

        return;
    }

    // both are vars
    if (!var1->isConstant() && !var2->isConstant()) {
        
        this->addPreparedCommand( var1->getLoadCommand() );
        
        this->addPreparedCommand( var2->getAddCommand() );

        return;
    }
}

void code_generator::Operations::sub(Var* var1, Var* var2) {

    this->checkVars(var1, var2);

    // var1 is number
    if (var1->isConstant()) {
        // and var2 is number
        if (var2->isConstant()) {

            int result = var1->getConstValue() - var2->getConstValue();
            if (result < 0) {
                result = 0;
            }

            this->addCommand( SET, std::to_string( result ) );
            
            return;
        }

        // var1 is number and var2 is var
        this->addCommand( SET, var1->getConstValueAsString() );
        
        this->addPreparedCommand( var2->getSubCommand() );

        return;
    }

    // var1 is var and var2 is number
    if (!var1->isConstant() && var2->isConstant()) {

        std::string constAddress = std::to_string( this->codeGen.getMemoryPointerForConst() );    

        this->addCommand( SET, var2->getConstValueAsString() );
        
        this->addCommand( STORE, constAddress );

        this->addPreparedCommand( var1->getLoadCommand() );

        this->addCommand( SUB, constAddress );

        return;
    }

    // both are vars
    if (!var1->isConstant() && !var2->isConstant()) {

        this->addPreparedCommand( var1->getLoadCommand() );

        this->addPreparedCommand( var2->getSubCommand() );

        return;
    }
}

void code_generator::Operations::mul(Var* var1, Var* var2) {
    
    this->checkVars(var1, var2);

    if (var2->isConstant()) {

        if (var1->isConstant()) {

            uint64_t result = var1->getConstValue() * var2->getConstValue();

            this->addCommand( SET, std::to_string(result) );

            return;
        }

        this->mulOneConstant(var1, var2->getConstValue());
        return;
    }

    if (var1->isConstant()) {
        this->mulOneConstant(var2, var1->getConstValue());
        return;
    }

    // both are variables
    std::string constAddress1 = std::to_string( this->codeGen.getMemoryPointerForConst());
    std::string a_temp_address = std::to_string( this->codeGen.getMemoryPointerForConst() + 1 );
    std::string b_temp_address = std::to_string( this->codeGen.getMemoryPointerForConst() + 2 );
    std::string wynik_temp_address = std::to_string( this->codeGen.getMemoryPointerForConst() + 3 );

    this->addPreparedCommand( var1->getLoadCommand() );
    this->addCommand( STORE, a_temp_address );

    this->addPreparedCommand( var2->getLoadCommand() );
    this->addCommand( STORE, b_temp_address );

    this->addCommand( SET, "0" );
    this->addCommand( STORE, wynik_temp_address );

    this->addCommand( LOAD, b_temp_address );
    this->addCommand( HALF );
    
    this->addCommand( ADD, "0" );
    
    this->addCommand( STORE, constAddress1 );

    this->addCommand( LOAD, b_temp_address );
    this->addCommand( SUB, constAddress1 );
    
    this->addCommand( JZERO, std::to_string( this->codeGen.getK() + 1 + 3 ) );

    this->addCommand( LOAD, wynik_temp_address );

    this->addCommand( ADD, a_temp_address );

    this->addCommand( STORE, wynik_temp_address );

    this->addCommand( LOAD, a_temp_address );
    this->addCommand( ADD, "0" );
    this->addCommand( STORE, a_temp_address );
    
    this->addCommand( LOAD, b_temp_address );
    this->addCommand( HALF );
    this->addCommand( STORE, b_temp_address );

    this->addCommand( JPOS, std::to_string( this->codeGen.getK() + 1 - 16 ) );
    this->addCommand( LOAD, wynik_temp_address );
    
}

void code_generator::Operations::mulOneConstant(Var* var, int const_value) {
    switch (const_value) {
    case 0:
        this->addCommand( SET, "0" );
        return;
        break;
    case 1:
        this->addPreparedCommand( var->getLoadCommand() );
        return;
        break;
    case 2:
        this->addPreparedCommand( var->getLoadCommand() );
        this->addCommand( ADD, "0" );
        return;
        break;
    default:
        std::string result_address = std::to_string( this->codeGen.getMemoryPointerForConst() );
        this->addCommand( SET, "0" );
        this->addCommand( STORE, result_address );
        for (int i = 0; i < const_value; i++) {
            this->addPreparedCommand( var->getLoadCommand() );
            this->addCommand( ADD, result_address );
            this->addCommand( STORE, result_address);
        }
        return;
    }
}

void code_generator::Operations::div(Var* var1, Var* var2) {
    
    this->checkVars(var1, var2);

    // std::cout << "Inside div";

    if (var2->isConstant()) {
        // std::cout << "is constant";
        if (var1->isConstant()) {
            int result = 0;

            if (var2->getConstValue() != 0) {
                result = var1->getConstValue() / var2->getConstValue();
            }

            this->addCommand( SET, std::to_string(result) );
            return;
        }

        this->divideByConstant(var1, var2->getConstValue());
        return;
    }

    // zaimplementowac dzielenie stalej przez zmienna

    // both are variables
    std::string a_temp_address = std::to_string( this->codeGen.getMemoryPointerForConst());
    std::string b_temp_address = std::to_string( this->codeGen.getMemoryPointerForConst() + 1 );
    std::string i_address = std::to_string( this->codeGen.getMemoryPointerForConst() + 2 );
    std::string result_address = std::to_string( this->codeGen.getMemoryPointerForConst() + 3 );

    this->addPreparedCommand( var1->getLoadCommand() );
    this->addCommand( JZERO, std::to_string( this->codeGen.getK() + 1 + 34 ) );
    this->addCommand( STORE, a_temp_address );

    this->addPreparedCommand( var2->getLoadCommand() );
    this->addCommand( JZERO, std::to_string( this->codeGen.getK() + 1 + 31 ) );
    this->addCommand( STORE, b_temp_address );

    this->addCommand( SET, "0" );
    this->addCommand( STORE, result_address );

    this->addCommand( SET, "1" );
    this->addCommand( STORE, i_address );

    this->addPreparedCommand( var2->getLoadCommand() );
    this->addCommand( SUB, a_temp_address );
    this->addCommand( JPOS, std::to_string( this->codeGen.getK() + 1 + 22 ) );

    this->addCommand( LOAD, b_temp_address );
    this->addCommand( ADD, "0" );
    this->addCommand( SUB, a_temp_address );
    this->addCommand( JZERO, std::to_string( this->codeGen.getK() + 1 + 11 ) );

    this->addCommand( LOAD, result_address );
    this->addCommand( ADD, i_address );
    this->addCommand( STORE, result_address );

    this->addCommand( LOAD, a_temp_address );
    this->addCommand( SUB, b_temp_address );
    this->addCommand( STORE, a_temp_address );

    this->addPreparedCommand( var2->getLoadCommand() );
    this->addCommand( STORE, b_temp_address );

    this->addCommand( SET, "1" );
    this->addCommand( STORE, i_address );

    this->addCommand( JUMP, std::to_string( this->codeGen.getK() + 1 + 6 ) );
    this->addCommand( LOAD, b_temp_address );
    this->addCommand( ADD, "0" );
    this->addCommand( STORE, b_temp_address );

    this->addCommand( LOAD, i_address );
    this->addCommand( ADD, "0" );
    this->addCommand( STORE, i_address );

    this->addCommand( JUMP, std::to_string( this->codeGen.getK() + 1 - 25 ) );
    this->addCommand( LOAD, result_address );

}

void code_generator::Operations::divideByConstant(Var* var, int const_value) {
    switch (const_value) {
        case 0:
            this->addCommand( SET, "0" );
            return;
        case 1:
            this->addPreparedCommand( var->getLoadCommand() );
            return;
        case 2:
            this->addPreparedCommand( var->getLoadCommand() );
            this->addCommand( HALF );
            return;
        default:
            std::string result_address = std::to_string( this->codeGen.getMemoryPointerForConst() );
            std::string a_temp_address = std::to_string( this->codeGen.getMemoryPointerForConst() + 1 );
            std::string b_temp_address = std::to_string( this->codeGen.getMemoryPointerForConst() + 2 );

            this->addPreparedCommand( var->getLoadCommand() );
            this->addCommand( STORE, a_temp_address );

            this->addCommand( SET, std::to_string( const_value ) );
            this->addCommand( STORE, b_temp_address );

            this->addCommand( SET, "0" );
            this->addCommand( STORE, result_address );

            this->addCommand( LOAD, b_temp_address );
            this->addCommand( SUB, a_temp_address );

            this->addCommand( JPOS, std::to_string( this->codeGen.getK() + 1 + 7 ) );

            this->addCommand( LOAD, a_temp_address );
            this->addCommand( SUB, b_temp_address );
            this->addCommand( STORE, a_temp_address );
            
            this->addCommand( SET, "1" );
            this->addCommand( ADD, result_address );
            this->addCommand( STORE, result_address );

            this->addCommand( JUMP, std::to_string( this->codeGen.getK() + 1 - 10 ) );

            this->addCommand( LOAD, result_address );

            return;
    }
}

void code_generator::Operations::mod(Var* var1, Var* var2) {

    this->checkVars(var1, var2);

    if (var2->isConstant()) {
        if (var1->isConstant()) {
            int result = var1->getConstValue() % var2->getConstValue();
            this->codeGen.commands.push_back(new Command(SET, std::to_string(result)));
            return;
        }

        this->modByConstant(var1, var2->getConstValue());
        return;
    }

    // both are variables

    std::string a_temp_address = std::to_string( this->codeGen.getMemoryPointerForConst());
    std::string b_temp_address = std::to_string( this->codeGen.getMemoryPointerForConst() + 1 );
    std::string i_address = std::to_string( this->codeGen.getMemoryPointerForConst() + 2 );
    std::string result_address = std::to_string( this->codeGen.getMemoryPointerForConst() + 3 );

    this->addPreparedCommand( var1->getLoadCommand() );
    this->addCommand( JZERO, std::to_string( this->codeGen.getK() + 1 + 34 ) );
    this->addCommand( STORE, a_temp_address );

    this->addPreparedCommand( var2->getLoadCommand() );
    this->addCommand( JZERO, std::to_string( this->codeGen.getK() + 1 + 31 ) );
    this->addCommand( STORE, b_temp_address );

    this->addCommand( SET, "0" );
    this->addCommand( STORE, result_address );

    this->addCommand( SET, "1" );
    this->addCommand( STORE, i_address );

    this->addPreparedCommand( var2->getLoadCommand() );
    this->addCommand( SUB, a_temp_address );
    this->addCommand( JPOS, std::to_string( this->codeGen.getK() + 1 + 22 ) );

    this->addCommand( LOAD, b_temp_address );
    this->addCommand( ADD, "0" );
    this->addCommand( SUB, a_temp_address );
    this->addCommand( JZERO, std::to_string( this->codeGen.getK() + 1 + 11 ) );

    this->addCommand( LOAD, result_address );
    this->addCommand( ADD, i_address );
    this->addCommand( STORE, result_address );

    this->addCommand( LOAD, a_temp_address );
    this->addCommand( SUB, b_temp_address );
    this->addCommand( STORE, a_temp_address );

    this->addPreparedCommand( var2->getLoadCommand() );
    this->addCommand( STORE, b_temp_address );

    this->addCommand( SET, "1" );
    this->addCommand( STORE, i_address );

    this->addCommand( JUMP, std::to_string( this->codeGen.getK() + 1 + 6 ) );
    this->addCommand( LOAD, b_temp_address );
    this->addCommand( ADD, "0" );
    this->addCommand( STORE, b_temp_address );

    this->addCommand( LOAD, i_address );
    this->addCommand( ADD, "0" );
    this->addCommand( STORE, i_address );

    this->addCommand( JUMP, std::to_string( this->codeGen.getK() + 1 - 25 ) );
    this->addCommand( LOAD, a_temp_address);
}

void code_generator::Operations::modByConstant(Var* var, int const_value) {
    std::string result_address = std::to_string( this->codeGen.getMemoryPointerForConst() );
    switch (const_value) {
        case 0:
            this->addPreparedCommand( var->getLoadCommand() );
            return;
        case 1:
            this->addCommand( SET, "0" );
            return;
        case 2:
            this->addPreparedCommand( var->getLoadCommand() );
            this->addCommand( HALF );
            this->addCommand( ADD, "0" );
            this->addCommand( STORE, result_address );
            this->addPreparedCommand( var->getLoadCommand() );
            this->addCommand( SUB, result_address );
            return;
        default:
            std::string a_temp_address = std::to_string( this->codeGen.getMemoryPointerForConst() + 1 );
            std::string b_temp_address = std::to_string( this->codeGen.getMemoryPointerForConst() + 2 );

            this->addPreparedCommand( var->getLoadCommand() );
            this->addCommand( STORE, a_temp_address );

            this->addCommand( SET, std::to_string( const_value ) );
            this->addCommand( STORE, b_temp_address );

            this->addCommand( SET, "0" );
            this->addCommand( STORE, result_address );

            this->addCommand( LOAD, b_temp_address );
            this->addCommand( SUB, a_temp_address );

            this->addCommand( JPOS, std::to_string( this->codeGen.getK() + 1 + 7 ) );

            this->addCommand( LOAD, a_temp_address );
            this->addCommand( SUB, b_temp_address );
            this->addCommand( STORE, a_temp_address );
            
            this->addCommand( SET, "1" );
            this->addCommand( ADD, result_address );
            this->addCommand( STORE, result_address );

            this->addCommand( JUMP, std::to_string( this->codeGen.getK() + 1 - 10 ) );

            this->addCommand( LOAD, a_temp_address );

            return;
    }
}