#include "code_generator.h"
#include <iostream>
using namespace std;

void code_generator::Operations::addCommand(CMD name, string param = "") {
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
    if ( this->codeGen.getInsideConditionFlag() == false ) {
        if ( !var1->isInitialized() ) {
            throw (string) "Variable '" + var1->getName() + "' wasn't initialized\n";
        } 
        
        if ( !var2->isInitialized() ) {
            throw (string) "Variable '" + var2->getName() + "' wasn't initialized\n";
        }
    }
}

void code_generator::Operations::add(Var* var1, Var* var2) {

    this->checkVars(var1, var2);

    if (var1->isConstant()) {
        if (var2->isConstant()) {
            int result = var1->getConstValue() + var2->getConstValue();

            this->addCommand(SET, to_string(result));

            return;
        }


        this->addCommand(SET, var1->getConstValueAsString() );
        
        this->addPreparedCommand( var2->getAddCommand() );

        return;
    }

    if (!var1->isConstant() && var2->isConstant()) {

        this->addCommand( SET, var2->getConstValueAsString() );

        this->addPreparedCommand( var1->getAddCommand() );

        return;
    }

    if (!var1->isConstant() && !var2->isConstant()) {
        
        this->addPreparedCommand( var1->getLoadCommand() );
        
        this->addPreparedCommand( var2->getAddCommand() );

        return;
    }
}

void code_generator::Operations::sub(Var* var1, Var* var2) {

    this->checkVars(var1, var2);

    if (var1->isConstant()) {
        if (var2->isConstant()) {

            int result = var1->getConstValue() - var2->getConstValue();
            if (result < 0) {
                result = 0;
            }

            this->addCommand( SET, to_string( result ) );
            
            return;
        }

        this->addCommand( SET, var1->getConstValueAsString() );
        
        this->addPreparedCommand( var2->getSubCommand() );

        return;
    }

    if (!var1->isConstant() && var2->isConstant()) {

        string constAddress = to_string( this->codeGen.getMemoryPointerForConst() );    

        this->addCommand( SET, var2->getConstValueAsString() );
        
        this->addCommand( STORE, constAddress );

        this->addPreparedCommand( var1->getLoadCommand() );

        this->addCommand( SUB, constAddress );

        return;
    }

    if (!var1->isConstant() && !var2->isConstant()) {

        this->addPreparedCommand( var1->getLoadCommand() );

        this->addPreparedCommand( var2->getSubCommand() );

        return;
    }
}

void code_generator::Operations::mul(Var* var1, Var* var2) {
    
    this->checkVars(var1, var2);

    string constAddress1 = to_string( this->codeGen.getMemoryPointerForConst());
    string a_temp_address = to_string( this->codeGen.getMemoryPointerForConst() + 1 );
    string b_temp_address = to_string( this->codeGen.getMemoryPointerForConst() + 2 );
    string wynik_temp_address = to_string( this->codeGen.getMemoryPointerForConst() + 3 );

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
    
    this->addCommand( JZERO, to_string( this->codeGen.getK() + 1 + 3 ) );

    this->addCommand( LOAD, wynik_temp_address );

    this->addCommand( ADD, a_temp_address );

    this->addCommand( STORE, wynik_temp_address );

    this->addCommand( LOAD, a_temp_address );
    this->addCommand( ADD, "0" );
    this->addCommand( STORE, a_temp_address );
    
    this->addCommand( LOAD, b_temp_address );
    this->addCommand( HALF );
    this->addCommand( STORE, b_temp_address );

    this->addCommand( JPOS, to_string( this->codeGen.getK() + 1 - 16 ) );
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
        string result_address = to_string( this->codeGen.getMemoryPointerForConst() );
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

    if (var2->isConstant()) {
        if (var1->isConstant()) {
            int result = 0;

            if (var2->getConstValue() != 0) {
                result = var1->getConstValue() / var2->getConstValue();
            }

            this->addCommand( SET, to_string(result) );
            return;
        }

        this->divideByConstant(var1, var2->getConstValue());
        return;
    }

    string a_temp_address = to_string( this->codeGen.getMemoryPointerForConst());
    string b_temp_address = to_string( this->codeGen.getMemoryPointerForConst() + 1 );
    string i_address = to_string( this->codeGen.getMemoryPointerForConst() + 2 );
    string result_address = to_string( this->codeGen.getMemoryPointerForConst() + 3 );

    this->addPreparedCommand( var1->getLoadCommand() );
    this->addCommand( JZERO, to_string( this->codeGen.getK() + 1 + 34 ) );
    this->addCommand( STORE, a_temp_address );

    this->addPreparedCommand( var2->getLoadCommand() );
    this->addCommand( JZERO, to_string( this->codeGen.getK() + 1 + 31 ) );
    this->addCommand( STORE, b_temp_address );

    this->addCommand( SET, "0" );
    this->addCommand( STORE, result_address );

    this->addCommand( SET, "1" );
    this->addCommand( STORE, i_address );

    this->addPreparedCommand( var2->getLoadCommand() );
    this->addCommand( SUB, a_temp_address );
    this->addCommand( JPOS, to_string( this->codeGen.getK() + 1 + 22 ) );

    this->addCommand( LOAD, b_temp_address );
    this->addCommand( ADD, "0" );
    this->addCommand( SUB, a_temp_address );
    this->addCommand( JZERO, to_string( this->codeGen.getK() + 1 + 11 ) );

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

    this->addCommand( JUMP, to_string( this->codeGen.getK() + 1 + 6 ) );
    this->addCommand( LOAD, b_temp_address );
    this->addCommand( ADD, "0" );
    this->addCommand( STORE, b_temp_address );

    this->addCommand( LOAD, i_address );
    this->addCommand( ADD, "0" );
    this->addCommand( STORE, i_address );

    this->addCommand( JUMP, to_string( this->codeGen.getK() + 1 - 25 ) );
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
            string result_address = to_string( this->codeGen.getMemoryPointerForConst() );
            string a_temp_address = to_string( this->codeGen.getMemoryPointerForConst() + 1 );
            string b_temp_address = to_string( this->codeGen.getMemoryPointerForConst() + 2 );

            this->addPreparedCommand( var->getLoadCommand() );
            this->addCommand( STORE, a_temp_address );

            this->addCommand( SET, to_string( const_value ) );
            this->addCommand( STORE, b_temp_address );

            this->addCommand( SET, "0" );
            this->addCommand( STORE, result_address );

            this->addCommand( LOAD, b_temp_address );
            this->addCommand( SUB, a_temp_address );

            this->addCommand( JPOS, to_string( this->codeGen.getK() + 1 + 7 ) );

            this->addCommand( LOAD, a_temp_address );
            this->addCommand( SUB, b_temp_address );
            this->addCommand( STORE, a_temp_address );
            
            this->addCommand( SET, "1" );
            this->addCommand( ADD, result_address );
            this->addCommand( STORE, result_address );

            this->addCommand( JUMP, to_string( this->codeGen.getK() + 1 - 10 ) );

            this->addCommand( LOAD, result_address );

            return;
    }
}

void code_generator::Operations::mod(Var* var1, Var* var2) {

    this->checkVars(var1, var2);

    if (var2->isConstant()) {
        if (var1->isConstant()) {
            int result = var1->getConstValue() % var2->getConstValue();
            this->codeGen.commands.push_back(new Command(SET, to_string(result)));
            return;
        }

        this->modByConstant(var1, var2->getConstValue());
        return;
    }

    string a_temp_address = to_string( this->codeGen.getMemoryPointerForConst());
    string b_temp_address = to_string( this->codeGen.getMemoryPointerForConst() + 1 );
    string i_address = to_string( this->codeGen.getMemoryPointerForConst() + 2 );
    string result_address = to_string( this->codeGen.getMemoryPointerForConst() + 3 );

    this->addPreparedCommand( var1->getLoadCommand() );
    this->addCommand( JZERO, to_string( this->codeGen.getK() + 1 + 34 ) );
    this->addCommand( STORE, a_temp_address );

    this->addPreparedCommand( var2->getLoadCommand() );
    this->addCommand( JZERO, to_string( this->codeGen.getK() + 1 + 31 ) );
    this->addCommand( STORE, b_temp_address );

    this->addCommand( SET, "0" );
    this->addCommand( STORE, result_address );

    this->addCommand( SET, "1" );
    this->addCommand( STORE, i_address );

    this->addPreparedCommand( var2->getLoadCommand() );
    this->addCommand( SUB, a_temp_address );
    this->addCommand( JPOS, to_string( this->codeGen.getK() + 1 + 22 ) );

    this->addCommand( LOAD, b_temp_address );
    this->addCommand( ADD, "0" );
    this->addCommand( SUB, a_temp_address );
    this->addCommand( JZERO, to_string( this->codeGen.getK() + 1 + 11 ) );

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

    this->addCommand( JUMP, to_string( this->codeGen.getK() + 1 + 6 ) );
    this->addCommand( LOAD, b_temp_address );
    this->addCommand( ADD, "0" );
    this->addCommand( STORE, b_temp_address );

    this->addCommand( LOAD, i_address );
    this->addCommand( ADD, "0" );
    this->addCommand( STORE, i_address );

    this->addCommand( JUMP, to_string( this->codeGen.getK() + 1 - 25 ) );
    this->addCommand( LOAD, a_temp_address);
}

void code_generator::Operations::modByConstant(Var* var, int const_value) {
    string result_address = to_string( this->codeGen.getMemoryPointerForConst() );
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
            string a_temp_address = to_string( this->codeGen.getMemoryPointerForConst() + 1 );
            string b_temp_address = to_string( this->codeGen.getMemoryPointerForConst() + 2 );

            this->addPreparedCommand( var->getLoadCommand() );
            this->addCommand( STORE, a_temp_address );

            this->addCommand( SET, to_string( const_value ) );
            this->addCommand( STORE, b_temp_address );

            this->addCommand( SET, "0" );
            this->addCommand( STORE, result_address );

            this->addCommand( LOAD, b_temp_address );
            this->addCommand( SUB, a_temp_address );

            this->addCommand( JPOS, to_string( this->codeGen.getK() + 1 + 7 ) );

            this->addCommand( LOAD, a_temp_address );
            this->addCommand( SUB, b_temp_address );
            this->addCommand( STORE, a_temp_address );
            
            this->addCommand( SET, "1" );
            this->addCommand( ADD, result_address );
            this->addCommand( STORE, result_address );

            this->addCommand( JUMP, to_string( this->codeGen.getK() + 1 - 10 ) );

            this->addCommand( LOAD, a_temp_address );

            return;
    }
}