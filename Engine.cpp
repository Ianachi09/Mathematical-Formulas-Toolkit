#include <vector>
#include <sstream>
#include <cmath>

#include "header.h"

// Math Parser part 1: Upgraded with PEMDAS (Order of Operations)
double evaluate(std::string expression) {

    // --- THE RECURSION BLOCK (PARENTHESES) ---
    // Step 1: Find the last opening bracket '('
    size_t openParen = expression.find_last_of('(');
    
    if (openParen != std::string::npos) {
        // Step 2: Find the matching closing bracket ')'
        size_t closeParen = expression.find(')', openParen);
        
        if (closeParen != std::string::npos) {
            // Extract the math inside the brackets (e.g., "5+5")
            std::string inside = expression.substr(openParen + 1, closeParen - openParen - 1);
            
            // MAGIC: The function calls ITSELF to solve the inside math!
            double insideResult = evaluate(inside);
            
            // Replace the "(5+5)" with "10.000" in the main string
            expression.replace(openParen, closeParen - openParen + 1, std::to_string(insideResult));
            
            // Restart the whole process with the new, simpler string
            return evaluate(expression); 
        }
    }
    // ----------------------------------------------
    std::stringstream ss(expression);
    std::vector<double> terms; // A list to hold our numbers before we add them
    double currentNum;
    char op;

    // Get the very first number and put it in our list
    ss >> currentNum;
    terms.push_back(currentNum);

    // Look for the next symbol and number
    while (ss >> op) {
        double nextNum;
        ss >> nextNum;

        if (op == '+') {
            terms.push_back(nextNum); // Save for later
        } 
        else if (op == '-') {
            terms.push_back(-nextNum); // Save as a negative for later
        } 
        else if (op == '*') {
            // Multiply the LAST number in our list immediately!
            terms.back() *= nextNum; 
        } 
        else if (op == '/') {
            // Divide the LAST number in our list immediately!
            if (nextNum != 0) terms.back() /= nextNum;
        }
        else if (op == '^') {
            // std::pow(base, exponent)
            terms.back() = std::pow(terms.back(), nextNum);
        }
    }

    // Finally, add everything in our list together
    double finalResult = 0;
    for (size_t i = 0; i < terms.size(); i++) {
        finalResult += terms[i];
    }
    
    return finalResult;
}

// Math Parser part 2, Variable swapper
std::string replaceVariable(std::string formula, char var, double value) {
    std::string valStr = std::to_string(value);
    size_t pos = formula.find(var);
    
    // While the letter exists in the string, replace it
    while (pos != std::string::npos) {
        formula.replace(pos, 1, valStr);
        pos = formula.find(var, pos + valStr.length());
    }
    return formula;
}