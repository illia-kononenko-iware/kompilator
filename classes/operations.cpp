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
        
        if (var2->isParameter()) {
            this->codeGen.commands.push_back(new Command(ADDI, 
                var2->getAddressAsString()
            ));
        } else {
            this->codeGen.commands.push_back(new Command(ADD, 
                var2->getAddressAsString()
            ));
        }

        return;
    }

    // var1 is var and var2 is number
    if (!var1->isConstant() && var2->isConstant()) {

        this->codeGen.commands.push_back(new Command(SET, 
            var2->getConstValueAsString()
        ));
        
        if (var1->isParameter()) {
            this->codeGen.commands.push_back(new Command(ADDI, 
                 var1->getAddressAsString()
            ));
        } else {
            this->codeGen.commands.push_back(new Command(ADD, 
                 var1->getAddressAsString()
            ));
        }

        return;
    }

    // both are vars
    if (!var1->isConstant() && !var2->isConstant()) {
        if (var1->isParameter()) {
            this->codeGen.commands.push_back(new Command(LOADI, 
                var1->getAddressAsString()
            ));
        } else {
            this->codeGen.commands.push_back(new Command(LOAD, 
                var1->getAddressAsString()
            ));
        }
        
        if (var2->isParameter()) {
            this->codeGen.commands.push_back(new Command(ADDI, 
                 var2->getAddressAsString()
            ));
        } else {
            this->codeGen.commands.push_back(new Command(ADD, 
                 var2->getAddressAsString()
            ));
        }

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
        
        if (var2->isParameter()) {
            this->codeGen.commands.push_back(new Command(SUBI, 
                var2->getAddressAsString()
            ));
        } else {
            this->codeGen.commands.push_back(new Command(SUB, 
                var2->getAddressAsString()
            ));
        }

        return;
    }

    // var1 is var and var2 is number
    if (!var1->isConstant() && var2->isConstant()) {
        int constAddress = this->codeGen.getMemoryPointerForConst();

        this->codeGen.commands.push_back(new Command(SET, 
            var2->getConstValueAsString()
        ));
        
        this->codeGen.commands.push_back(new Command(STORE, 
            std::to_string(constAddress)
        ));
        
        if (var1->isParameter()) {
            this->codeGen.commands.push_back(new Command(LOADI, 
                var1->getAddressAsString()
            ));
        } else {
            this->codeGen.commands.push_back(new Command(LOAD, 
                var1->getAddressAsString()
            ));
        }

        this->codeGen.commands.push_back(new Command(SUB, 
            std::to_string(constAddress)
        ));
        return;
    }

    // both are vars
    if (!var1->isConstant() && !var2->isConstant()) {
        if (var1->isParameter()) {
            this->codeGen.commands.push_back(new Command(LOADI, 
                var1->getAddressAsString()
            ));
        } else {
            this->codeGen.commands.push_back(new Command(LOAD, 
                var1->getAddressAsString()
            ));
        }
        if (var2->isParameter()) {
            this->codeGen.commands.push_back(new Command(SUBI, 
                var2->getAddressAsString()
            ));
        } else {
            this->codeGen.commands.push_back(new Command(SUB, 
                var2->getAddressAsString()
            ));
        }
        return;
    }
}

void code_generator::Operations::mul(Var* var1, Var* var2) {
    if (var2->isConstant()) {
        if (var1->isConstant()) {
            uint64_t result = var1->getConstValue() * var2->getConstValue();
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
    std::string constAddress1 = std::to_string( this->codeGen.getMemoryPointerForConst());
    std::string a_temp_address = std::to_string( this->codeGen.getMemoryPointerForConst() + 1 );
    std::string b_temp_address = std::to_string( this->codeGen.getMemoryPointerForConst() + 2 );
    std::string wynik_temp_address = std::to_string( this->codeGen.getMemoryPointerForConst() + 3 );

    if (var1->isParameter()) {
        this->codeGen.commands.push_back(new Command( LOADI, var1->getAddressAsString() ));
    } else {
        this->codeGen.commands.push_back(new Command( LOAD, var1->getAddressAsString() ));
    }

    this->codeGen.commands.push_back(new Command( STORE, a_temp_address ));

    if (var2->isParameter()) {
        this->codeGen.commands.push_back(new Command( LOADI, var2->getAddressAsString() ));
    } else {
        this->codeGen.commands.push_back(new Command( LOAD, var2->getAddressAsString() ));
    }

    this->codeGen.commands.push_back(new Command( STORE, b_temp_address ));

    this->codeGen.commands.push_back(new Command( SET, std::to_string(0) ));
    this->codeGen.commands.push_back(new Command( STORE, wynik_temp_address ));

    this->codeGen.commands.push_back(new Command( LOAD, b_temp_address ));
    this->codeGen.commands.push_back(new Command( HALF ));
    
    this->codeGen.commands.push_back(new Command( ADD, std::to_string( 0 ) ));
    
    this->codeGen.commands.push_back(new Command( STORE, constAddress1 ));

    this->codeGen.commands.push_back(new Command( LOAD, b_temp_address ));
    this->codeGen.commands.push_back(new Command( SUB, constAddress1 ));
    
    this->codeGen.commands.push_back(new Command( JZERO, std::to_string( this->codeGen.getK() + 1 + 3 ) ) );

    this->codeGen.commands.push_back(new Command( LOAD, wynik_temp_address ));

    this->codeGen.commands.push_back(new Command( ADD, a_temp_address ));

    this->codeGen.commands.push_back(new Command( STORE, wynik_temp_address ));

    this->codeGen.commands.push_back(new Command( LOAD, a_temp_address ));
    this->codeGen.commands.push_back(new Command( ADD, std::to_string( 0 ) ));
    this->codeGen.commands.push_back(new Command( STORE, a_temp_address ));
    
    this->codeGen.commands.push_back(new Command( LOAD, b_temp_address ));
    this->codeGen.commands.push_back(new Command( HALF ));
    this->codeGen.commands.push_back(new Command( STORE, b_temp_address ));

    this->codeGen.commands.push_back(new Command( JPOS, std::to_string( this->codeGen.getK() + 1 - 16 ) ));
    this->codeGen.commands.push_back(new Command( LOAD, wynik_temp_address ));
    
}

void code_generator::Operations::mulOneConstant(Var* var, int const_value) {
    switch (const_value) {
    case 0:
        this->codeGen.commands.push_back(new Command(SET, std::to_string(0)));
        return;
        break;
    case 1:
        if (var->isParameter()) {
            this->codeGen.commands.push_back(new Command(LOADI, var->getAddressAsString() ));
        } else {
            this->codeGen.commands.push_back(new Command(LOAD, var->getAddressAsString() ));
        }
        return;
        break;
    case 2:
        if (var->isParameter()) {
            this->codeGen.commands.push_back(new Command(LOADI, var->getAddressAsString() ));
        } else {
            this->codeGen.commands.push_back(new Command(LOAD, var->getAddressAsString() ));
        }
        this->codeGen.commands.push_back(new Command(ADD, std::to_string(0)));
        return;
        break;
    default:
        if (var->isParameter()) {
            this->codeGen.commands.push_back(new Command(LOADI, var->getAddressAsString() ));
        } else {
            this->codeGen.commands.push_back(new Command(LOAD, var->getAddressAsString() ));
        }
        for (int i = 1; i < const_value; i++) {
            this->codeGen.commands.push_back(new Command(ADD, std::to_string(0) ));
            if (var->isParameter()) {
                this->codeGen.commands.push_back(new Command(STOREI, var->getAddressAsString() ));
            } else {
                this->codeGen.commands.push_back(new Command(STORE, var->getAddressAsString() ));
            }
            
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
    std::string a_temp_address = std::to_string( this->codeGen.getMemoryPointerForConst());
    std::string b_temp_address = std::to_string( this->codeGen.getMemoryPointerForConst() + 1 );
    std::string i_address = std::to_string( this->codeGen.getMemoryPointerForConst() + 2 );
    std::string result_address = std::to_string( this->codeGen.getMemoryPointerForConst() + 3 );

    this->codeGen.commands.push_back(new Command( LOAD, var1->getAddressAsString() ));
    this->codeGen.commands.push_back(new Command( STORE, a_temp_address ));

    this->codeGen.commands.push_back(new Command( LOAD, var2->getAddressAsString() ));
    this->codeGen.commands.push_back(new Command( STORE, b_temp_address ));

    this->codeGen.commands.push_back(new Command( SET, "0" ));
    this->codeGen.commands.push_back(new Command( STORE, result_address ));

    this->codeGen.commands.push_back(new Command( SET, "1" ));
    this->codeGen.commands.push_back(new Command( STORE, i_address ));

    this->codeGen.commands.push_back(new Command( LOAD, var2->getAddressAsString() ));
    this->codeGen.commands.push_back(new Command( SUB, a_temp_address ));
    this->codeGen.commands.push_back(new Command( JPOS, std::to_string( this->codeGen.getK() + 1 + 22 ) ) );

    this->codeGen.commands.push_back(new Command( LOAD, b_temp_address ));
    this->codeGen.commands.push_back(new Command( ADD, "0" ));
    this->codeGen.commands.push_back(new Command( SUB, a_temp_address ));
    this->codeGen.commands.push_back(new Command( JZERO, std::to_string( this->codeGen.getK() + 1 + 11 ) ) );

    this->codeGen.commands.push_back(new Command( LOAD, result_address ));
    this->codeGen.commands.push_back(new Command( ADD, i_address ));
    this->codeGen.commands.push_back(new Command( STORE, result_address ));

    this->codeGen.commands.push_back(new Command( LOAD, a_temp_address ));
    this->codeGen.commands.push_back(new Command( SUB, b_temp_address ));
    this->codeGen.commands.push_back(new Command( STORE, a_temp_address ));

    this->codeGen.commands.push_back(new Command( LOAD, var2->getAddressAsString() ));
    this->codeGen.commands.push_back(new Command( STORE, b_temp_address ));

    this->codeGen.commands.push_back(new Command( SET, "1" ));
    this->codeGen.commands.push_back(new Command( STORE, i_address ));

    this->codeGen.commands.push_back(new Command( JUMP, std::to_string( this->codeGen.getK() + 1 + 6 ) ) );
    this->codeGen.commands.push_back(new Command( LOAD, b_temp_address ));
    this->codeGen.commands.push_back(new Command( ADD, "0" ));
    this->codeGen.commands.push_back(new Command( STORE, b_temp_address ));

    this->codeGen.commands.push_back(new Command( LOAD, i_address ));
    this->codeGen.commands.push_back(new Command( ADD, "0" ));
    this->codeGen.commands.push_back(new Command( STORE, i_address ));

    this->codeGen.commands.push_back(new Command( JUMP, std::to_string( this->codeGen.getK() + 1 - 25 ) ) );
    this->codeGen.commands.push_back(new Command( LOAD, result_address ));
}

void code_generator::Operations::divideByConstant(Var* var1, int const_value) {
    switch (const_value) {
        case 0:
            this->codeGen.commands.push_back(new Command(SET, std::to_string(0)));
            return;
        case 1:
            if (var1->isParameter()) {
                this->codeGen.commands.push_back(new Command(LOADI, var1->getAddressAsString() ));
            } else {
                this->codeGen.commands.push_back(new Command(LOAD, var1->getAddressAsString() ));
            }
            return;
        case 2:
            if (var1->isParameter()) {
                this->codeGen.commands.push_back(new Command(LOADI, var1->getAddressAsString() ));
            } else {
                this->codeGen.commands.push_back(new Command(LOAD, var1->getAddressAsString() ));
            }
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

    std::string a_temp_address = std::to_string( this->codeGen.getMemoryPointerForConst());
    std::string b_temp_address = std::to_string( this->codeGen.getMemoryPointerForConst() + 1 );
    std::string i_address = std::to_string( this->codeGen.getMemoryPointerForConst() + 2 );
    std::string result_address = std::to_string( this->codeGen.getMemoryPointerForConst() + 3 );

    this->codeGen.commands.push_back(new Command( LOAD, var1->getAddressAsString() ));
    this->codeGen.commands.push_back(new Command( STORE, a_temp_address ));

    this->codeGen.commands.push_back(new Command( LOAD, var2->getAddressAsString() ));
    this->codeGen.commands.push_back(new Command( STORE, b_temp_address ));

    this->codeGen.commands.push_back(new Command( SET, "0" ));
    this->codeGen.commands.push_back(new Command( STORE, result_address ));

    this->codeGen.commands.push_back(new Command( SET, "1" ));
    this->codeGen.commands.push_back(new Command( STORE, i_address ));

    this->codeGen.commands.push_back(new Command( LOAD, var2->getAddressAsString() ));
    this->codeGen.commands.push_back(new Command( SUB, a_temp_address ));
    this->codeGen.commands.push_back(new Command( JPOS, std::to_string( this->codeGen.getK() + 1 + 22 ) ) );

    this->codeGen.commands.push_back(new Command( LOAD, b_temp_address ));
    this->codeGen.commands.push_back(new Command( ADD, "0" ));
    this->codeGen.commands.push_back(new Command( SUB, a_temp_address ));
    this->codeGen.commands.push_back(new Command( JZERO, std::to_string( this->codeGen.getK() + 1 + 11 ) ) );

    this->codeGen.commands.push_back(new Command( LOAD, result_address ));
    this->codeGen.commands.push_back(new Command( ADD, i_address ));
    this->codeGen.commands.push_back(new Command( STORE, result_address ));

    this->codeGen.commands.push_back(new Command( LOAD, a_temp_address ));
    this->codeGen.commands.push_back(new Command( SUB, b_temp_address ));
    this->codeGen.commands.push_back(new Command( STORE, a_temp_address ));

    this->codeGen.commands.push_back(new Command( LOAD, var2->getAddressAsString() ));
    this->codeGen.commands.push_back(new Command( STORE, b_temp_address ));

    this->codeGen.commands.push_back(new Command( SET, "1" ));
    this->codeGen.commands.push_back(new Command( STORE, i_address ));

    this->codeGen.commands.push_back(new Command( JUMP, std::to_string( this->codeGen.getK() + 1 + 6 ) ) );
    this->codeGen.commands.push_back(new Command( LOAD, b_temp_address ));
    this->codeGen.commands.push_back(new Command( ADD, "0" ));
    this->codeGen.commands.push_back(new Command( STORE, b_temp_address ));

    this->codeGen.commands.push_back(new Command( LOAD, i_address ));
    this->codeGen.commands.push_back(new Command( ADD, "0" ));
    this->codeGen.commands.push_back(new Command( STORE, i_address ));

    this->codeGen.commands.push_back(new Command( JUMP, std::to_string( this->codeGen.getK() + 1 - 25 ) ) );
    this->codeGen.commands.push_back(new Command( LOAD, a_temp_address ));
}