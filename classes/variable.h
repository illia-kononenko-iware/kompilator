#ifndef VARIABLE_H
#define VARIABLE_H

#include <string>
#include <iostream>
#include "command.h"

class Var {
    public:

        Var(std::string name, int address, bool isParameter) {
            // std::cout << "Inside variable constructor\n";
            this->name = name;
            this->address = address;
            this->value = 0;
            this->isConstant_flag = false;
            this->initialized = false;
            this->isParameterFlag = isParameter;
        }

        Var(uint64_t value) {
            // std::cout << "Inside constant constructor\n";
            this->name = "const";
            this->address = 0;
            this->value = value;
            // std::cout << "Value: " << this->getConstValue() << std::endl;
            this->isConstant_flag = true;
            this->initialized = true;
        }

        int getAddress() { return this->address; }

        std::string getAddressAsString() { return std::to_string(this->address); }

        Command* getLoadCommand() {
            if (this->isParameter()) {
                return new Command( LOADI, this->getAddressAsString() );
            } else {
                return new Command( LOAD, this->getAddressAsString() );
            }
        }

        Command* getAddCommand() {
            if (this->isParameter()) {
                return new Command( ADDI, this->getAddressAsString() );
            } else {
                return new Command( ADD, this->getAddressAsString() );
            }
        }
        
        Command* getSubCommand() {
            if (this->isParameter()) {
                return new Command( SUBI, this->getAddressAsString() );
            } else {
                return new Command( SUB, this->getAddressAsString() );
            }
        }

        Command* getStoreCommand() {
            if (this->isParameter()) {
                return new Command( STOREI, this->getAddressAsString() );
            } else {
                return new Command( STORE, this->getAddressAsString() );
            }
        }

        bool isConstant() { return this->isConstant_flag; }

        std::string getName() { return this->name; }

        uint64_t getConstValue() { return this->value; }

        std::string getConstValueAsString() { return std::to_string(this->value); }

        void initialize() { this->initialized = true; }

        bool isInitialized() { return this->initialized; }

        bool isParameter() { return this->isParameterFlag; }

    private:
        std::string name;
        int address;
        uint64_t value;
        bool isConstant_flag;
        bool initialized;
        bool isParameterFlag = false;
};

#endif