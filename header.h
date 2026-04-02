#pragma once
#include <string>

// 1. The Structure
struct Formula { 
    std::string name;
    std::string equation;
    std::string explaination;
};

// 2. The Function Signatures (Notice there is no actual code here, just semicolons!)
double evaluate(std::string expression);
std::string replaceVariable(std::string formula, char var, double value);