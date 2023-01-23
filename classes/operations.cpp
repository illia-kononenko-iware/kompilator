#include "code_generator.h"
#include <iostream>

// finish for consts from both sides

void code_generator::Operations::add(Var* var1, Var* var2) {
    // var1 is number
    if (var1->isConstant()) {
        // and var2 is number
        if (var2->isConstant()) {
            int result = var1->getConstValue() + var2->getConstValue();
            this->codeGen.commands.push_back(new Command(SET, 
                std::to_string(result)
            ));
            return;
        }

        // var1 is number and var2 is var

        this->codeGen.commands.push_back(new Command(SET, 
            var1->getConstValueAsString()
        ));
        this->codeGen.commands.push_back(new Command(ADD, 
            var2->getAddressAsString()
        ));
        return;
    }

    // var1 is var and var2 is number
    if (!var1->isConstant() && var2->isConstant()) {

        this->codeGen.commands.push_back(new Command(SET, 
            var2->getConstValueAsString()
        ));
        
        this->codeGen.commands.push_back(new Command(ADD, 
            var1->getAddressAsString()
        ));

        return;
    }

    // both are vars
    if (!var1->isConstant() && !var2->isConstant()) {
        this->codeGen.commands.push_back(new Command(LOAD, 
            var1->getConstValueAsString()
        ));
        this->codeGen.commands.push_back(new Command(ADD, 
            var2->getAddressAsString()
        ));
        return;
    }
}

void code_generator::Operations::sub(Var* var1, Var* var2) {
    // var1 is number
    if (var1->isConstant()) {
        // and var2 is number
        if (var2->isConstant()) {
            int result = var1->getConstValue() - var2->getConstValue();
            if (result < 0) {
                result = 0;
            }
            this->codeGen.commands.push_back(new Command(SET, 
                std::to_string(result)
            ));
            return;
        }

        // var1 is number and var2 is var

        this->codeGen.commands.push_back(new Command(SET, 
            var1->getConstValueAsString()
        ));
        this->codeGen.commands.push_back(new Command(SUB, 
            var2->getAddressAsString()
        ));
        return;
    }

    // var1 is var and var2 is number
    if (!var1->isConstant() && var2->isConstant()) {
        int constAddress = this->codeGen.getLatestMemoryPoint();

        this->codeGen.commands.push_back(new Command(SET, 
            var2->getConstValueAsString()
        ));
        
        this->codeGen.commands.push_back(new Command(STORE, 
            std::to_string(constAddress)
        ));
        
        this->codeGen.commands.push_back(new Command(LOAD, 
            var1->getAddressAsString()
        ));

        this->codeGen.commands.push_back(new Command(SUB, 
            std::to_string(constAddress)
        ));
        return;
    }

    // both are vars
    if (!var1->isConstant() && !var2->isConstant()) {
        this->codeGen.commands.push_back(new Command(LOAD, 
            var1->getConstValueAsString()
        ));
        this->codeGen.commands.push_back(new Command(SUB, 
            var2->getAddressAsString()
        ));
        return;
    }
}

void code_generator::Operations::mul(Var* var1, Var* var2) {
    if (var2->isConstant()) {
        if (var1->isConstant()) {
            int result = var1->getConstValue() / var2->getConstValue();
            this->codeGen.commands.push_back(new Command(SET, std::to_string(result)));
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
    std::string constAddress1 = std::to_string( this->codeGen.getLatestMemoryPoint());
    std::string constAddress2 = std::to_string( this->codeGen.getLatestMemoryPoint() + 1 );
    std::string constAddress3 = std::to_string( this->codeGen.getLatestMemoryPoint() + 2 );

    this->codeGen.commands.push_back(new Command( LOAD, var1->getAddressAsString() ));
    this->codeGen.commands.push_back(new Command( STORE, constAddress1 ));
    
    this->codeGen.commands.push_back(new Command( JZERO, std::to_string( this->codeGen.getK() + 1 + 15 ) ));
    
    this->codeGen.commands.push_back(new Command( LOAD, var2->getAddressAsString() ));
    this->codeGen.commands.push_back(new Command( STORE, constAddress2 ));

    this->codeGen.commands.push_back(new Command( JZERO, std::to_string( this->codeGen.getK() + 1 + 12 ) ));
    
    this->codeGen.commands.push_back(new Command( SET, std::to_string(1) ));
    this->codeGen.commands.push_back(new Command( STORE, constAddress3 ));

    this->codeGen.commands.push_back(new Command( LOAD, constAddress2 ));
    this->codeGen.commands.push_back(new Command( SUB, constAddress3 ));
    this->codeGen.commands.push_back(new Command( JZERO, std::to_string( this->codeGen.getK() + 1 + 6 ) ));

    this->codeGen.commands.push_back(new Command( STORE, constAddress2 ));
    this->codeGen.commands.push_back(new Command( LOAD, var1->getAddressAsString() ));
    this->codeGen.commands.push_back(new Command( ADD, constAddress1 ));
    this->codeGen.commands.push_back(new Command( STORE, constAddress1 ));

    this->codeGen.commands.push_back(new Command( JUMP, std::to_string( this->codeGen.getK() + 1 - 7 ) ));

    this->codeGen.commands.push_back(new Command( LOAD, constAddress1 ));
    
}

void code_generator::Operations::mulOneConstant(Var* var, int const_value) {
    switch (const_value) {
    case 0:
        this->codeGen.commands.push_back(new Command(SET, std::to_string(0)));
        return;
        break;
    case 1:
        this->codeGen.commands.push_back(new Command(LOAD, var->getAddressAsString() ));
        return;
        break;
    case 2:
        this->codeGen.commands.push_back(new Command(LOAD, var->getAddressAsString() ));
        this->codeGen.commands.push_back(new Command(ADD, std::to_string(0)));
        return;
        break;
    default:
        this->codeGen.commands.push_back(new Command(LOAD, var->getAddressAsString() ));
        for (int i = 1; i < const_value; i++) {
            this->codeGen.commands.push_back(new Command(ADDI, var->getAddressAsString() ));
        }
        return;
    }
}

void code_generator::Operations::div(Var* var1, Var* var2) {

    // std::cout << "Inside div";

    if (var2->isConstant()) {
        // std::cout << "is constant";
        if (var1->isConstant()) {
            int result = 0;
            if (var2->getConstValue() != 0) {
                result = var1->getConstValue() / var2->getConstValue();
            }
            this->codeGen.commands.push_back(new Command(SET, std::to_string(result)));
            return;
        }

        this->divideByConstant(var1, var2->getConstValue());
        return;
    }

    // both are variables
    std::string constAddress1 = std::to_string( this->codeGen.getLatestMemoryPoint());
    std::string constAddress2 = std::to_string( this->codeGen.getLatestMemoryPoint() + 1 );

    this->codeGen.commands.push_back(new Command( LOAD, var2->getAddressAsString() ));
    this->codeGen.commands.push_back(new Command( JZERO, std::to_string( this->codeGen.getK() + 1 + 25 ) ));
    this->codeGen.commands.push_back(new Command( SUB, var1->getAddressAsString() ));
    this->codeGen.commands.push_back(new Command( JPOS, std::to_string( this->codeGen.getK() + 1 + 2 ) ));
    
    this->codeGen.commands.push_back(new Command( JUMP, std::to_string( this->codeGen.getK() + 1 + 3 ) ));
    this->codeGen.commands.push_back(new Command( SET, std::to_string( 0 ) ));
    this->codeGen.commands.push_back(new Command( JUMP, std::to_string( this->codeGen.getK() + 1 + 20 ) ));

    this->codeGen.commands.push_back(new Command( LOAD, var1->getAddressAsString() ));
    this->codeGen.commands.push_back(new Command( SUB, var2->getAddressAsString() ));
    this->codeGen.commands.push_back(new Command( JPOS, std::to_string( this->codeGen.getK() + 1 + 3 ) ));
    
    this->codeGen.commands.push_back(new Command( SET, std::to_string( 1 ) ));
    this->codeGen.commands.push_back(new Command( JUMP, std::to_string( this->codeGen.getK() + 1 + 15 ) ));
    
    this->codeGen.commands.push_back(new Command( STORE, constAddress1 ));
    this->codeGen.commands.push_back(new Command( SET, std::to_string( 1 ) ));
    this->codeGen.commands.push_back(new Command( STORE, constAddress2 ));

    this->codeGen.commands.push_back(new Command( LOAD, var2->getAddressAsString() ));
    this->codeGen.commands.push_back(new Command( SUB, constAddress1 ));
    this->codeGen.commands.push_back(new Command( JPOS, std::to_string( this->codeGen.getK() + 1 + 8 ) ));

    this->codeGen.commands.push_back(new Command( SET, std::to_string( 1 ) ));
    this->codeGen.commands.push_back(new Command( ADD, constAddress2 ));
    this->codeGen.commands.push_back(new Command( STORE, constAddress2 ));

    this->codeGen.commands.push_back(new Command( LOAD, constAddress1 ));
    this->codeGen.commands.push_back(new Command( SUB, var2->getAddressAsString() ));
    this->codeGen.commands.push_back(new Command( STORE, constAddress1 ));

    this->codeGen.commands.push_back(new Command( JUMP, std::to_string( this->codeGen.getK() + 1 - 9 ) ));

    this->codeGen.commands.push_back(new Command( LOAD, constAddress2 ));

}

void code_generator::Operations::divideByConstant(Var* var1, int const_value) {
    switch (const_value) {
        case 0:
            this->codeGen.commands.push_back(new Command(SET, std::to_string(0)));
            return;
        case 1:
            this->codeGen.commands.push_back(new Command(LOAD, var1->getAddressAsString() ));
            return;
        case 2:
            this->codeGen.commands.push_back(new Command(LOAD, var1->getAddressAsString() ));
            this->codeGen.commands.push_back(new Command(HALF));
            return;
        default:
            return;
    }
}

void code_generator::Operations::mod(Var* var1, Var* var2) {
    if (var2->isConstant()) {
        if (var1->isConstant()) {
            int result = var1->getConstValue() % var2->getConstValue();
            this->codeGen.commands.push_back(new Command(SET, std::to_string(result)));
            return;
        }

    }

    // both are variables
    std::string constAddress1 = std::to_string( this->codeGen.getLatestMemoryPoint());
    std::string constAddress2 = std::to_string( this->codeGen.getLatestMemoryPoint() + 1 );
    std::string constAddress3 = std::to_string( this->codeGen.getLatestMemoryPoint() + 2 );

    this->codeGen.commands.push_back(new Command( LOAD, var1->getAddressAsString() ));
    this->codeGen.commands.push_back(new Command( STORE, constAddress1 ));

    this->codeGen.commands.push_back(new Command( LOAD, var2->getAddressAsString() ));
    this->codeGen.commands.push_back(new Command( SUB, constAddress1 ));
    
    this->codeGen.commands.push_back(new Command( JPOS, std::to_string( this->codeGen.getK() + 1 + 2 ) ));
    this->codeGen.commands.push_back(new Command( JUMP, std::to_string( this->codeGen.getK() + 1 + 3 ) ));

    this->codeGen.commands.push_back(new Command( LOAD, constAddress1 ));
    this->codeGen.commands.push_back(new Command( JUMP, std::to_string( this->codeGen.getK() + 1 + 6 ) ));
    
    this->codeGen.commands.push_back(new Command( LOAD, constAddress1 ));
    this->codeGen.commands.push_back(new Command( SUB, var2->getAddressAsString() ));

    this->codeGen.commands.push_back(new Command( JZERO, std::to_string( this->codeGen.getK() + 1 + 3 ) ));
    this->codeGen.commands.push_back(new Command( STORE, constAddress1 ));
    this->codeGen.commands.push_back(new Command( JUMP, std::to_string( this->codeGen.getK() + 1 - 10 ) ));
}