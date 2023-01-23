#ifndef VARIABLE_H
#define VARIABLE_H

#include <string>
#include <iostream>

class Var {
    public:

        Var(std::string name, int address) {
            // std::cout << "Inside variable constructor\n";
            this->name = name;
            this->address = address;
            this->value = 0;
            this->isConstant_flag = false;
            this->initialized = false;
        }

        Var(int value) {
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

        bool isConstant() { return this->isConstant_flag; }

        std::string getName() { return this->name; }

        int getConstValue() { return this->value; }

        std::string getConstValueAsString() { return std::to_string(this->value); }

        void initialize() { this->initialized = true; }

        bool isInitialized() { return this->initialized; }

    private:
        std::string name;
        int address;
        int value;
        bool isConstant_flag;
        bool initialized;
};

#endif