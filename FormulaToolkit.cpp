#include <iostream>
#include <vector>
#include <fstream>
#include <limits>
#include <cmath>

#include "header.h"

int main() {
    // 
    std::vector <Formula> mathList; // Makes a space that uses 'Formula' struct rule

    std::ifstream file("formulas.txt"); // Opens the file
    if (file.is_open()) {
        std::string tempName, tempEq, tempExp; // Makes a space for the formula to load
        while (std::getline(file, tempName, '|')) {
            std::getline(file, tempEq, '|'); // getline works like this 'getline(where to read from, where to save, what character to stop reading on)
            std::getline(file, tempExp, '\n'); // For this data, we stops at new line (\n) as its the last data

            mathList.push_back({tempName, tempEq, tempExp}); // push_back adds the elements to the end of vector
        }
    file.close(); // You should always close files after finished using
    } else {
        std::cout << "Error: No formula file exists" << std::endl;
    }

    bool isRunning = true;

    while (isRunning) {
        std::cout << "=====================================================\n";
        std::cout << "              Mathemathical Formula List             \n";
        std::cout << "=====================================================\n";

        // List down all formulas read from the file
        for (size_t i=0; i<mathList.size(); i++) {
            std::cout << "[" << i+1 << "]" << mathList[i].name << std::endl; 
        }
        std::cout << "\n[" << mathList.size() + 1 << "] Add New Formula\n";
        std::cout << "[0] Exit Program\n";
        std::cout << "-----------------------------------------------------\n";

        // Prompting the users for choice
        int choice;
        std::cout << "Select an option : ";
        std::cin >> choice;
        // Safety net for cin function
        if (std::cin.fail()) {
            std::cin.clear(); // Clear the panic state 
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Throw away the bad input (data other than integers, what cin is asking for)
            std::cout << "Error: Please enter a number.\n";
            continue;
        }

        
        if (choice == 0) { // Terminating the program if being prompted 0
            std::cout << "\nExiting Program. . .";
            isRunning = false;
        } else if (choice == mathList.size() + 1) { // Function to write new formulas if prompted second last index
            system("cls");
            std::cout << "=====================================================\n";
            std::cout << "Adding New Formula";
            std::cout << "Remember: Use lowercase variables and explicit math (e.g., a * b).\n\n";

            std::string newName, newEq, newExp; // Temporary space to keep all those data so it can be put into the file at once

            std::cout << "Enter Formula Name (e.g., Density): ";
            // std::ws tells the computer to ignore the leftover "Enter" key press from the menu
            std::getline(std::cin >> std::ws, newName); 

            std::cout << "Enter Equation (e.g., m / v): ";
            std::getline(std::cin, newEq);

            std::cout << "Enter Explanation: ";
            std::getline(std::cin, newExp);

            // 1. Open the file in "Append" mode (std::ios::app)
            // If you don't use append, it will delete everything and write ONLY this new formula!
            std::ofstream outFile("formulas.txt", std::ios::app);
            
            if (outFile.is_open()) {
                // 2. Write the new string to the text file using our pipe | format
                outFile << "\n" << newName << "|" << newEq << "|" << newExp;
                outFile.close();

                // 3. Add it to the program's live memory so you don't have to restart the app
                mathList.push_back({newName, newEq, newExp});

                std::cout << "\nSuccess! Formula permanently saved to file.\n";
            } else {
                std::cout << "\nError: Could not open file for writing.\n";
            }

            std::cout << "\nPress Enter to return to main menu...";
            std::cin.get(); // Waits for the user to press Enter
            system("cls");
        } else if (choice > 0 && choice <= mathList.size()) { //Gets the index if other than that
            int index = choice - 1; // -1 because array starts with 0
            system("cls");
            
            bool inSubMenu = true;

            while (inSubMenu) {
                std::cout << std::endl;
                std::cout << std::endl;
                std::cout << mathList[index].name << " \n";
                std::cout << "=====================================================\n";
                std::cout << "[1] Show Formula and Explaination\n";
                std::cout << "[2] Use the formula\n";
                std::cout << "[3] Go back to Main Menu\n";
                std::cout << "-----------------------------------------------------\n";

                // Prompting the users for subchoice
                int subChoice;
                std::cout << "Select an option : ";
                std::cin >> subChoice;
                // Safety net for cin function
                if (std::cin.fail()) { 
                    std::cin.clear();  // Clear the panic state 
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Throw away the bad input (data other than integers, what cin is asking for)
                    std::cout << "Error: Please enter a number.\n";
                    continue; 
                }

                switch (subChoice) {
                    case 1:
                    system("cls");
                    std::cout << "Equation : " << mathList[index].equation << std::endl;
                    std::cout << "Explanation : " << mathList[index].explaination << std::endl;
                    std::cout << std::endl;
                    break;

                    case 2: {
                    system("cls");
                    std::string formulaText = mathList[index].equation;
                    std::cout << "\n--- Processing: " << mathList[index].name << " ---\n";
                    std::cout << "Formula: " << formulaText << "\n\n";

                    // 1. Scan for variables (a-z) and ask user for values
                    for (char c = 'a'; c <= 'z'; ++c) {
                        if (formulaText.find(c) != std::string::npos) {
                            double userVal;
                            std::cout << "Enter value for '" << c << "': ";
                            
                            // Safety check for number input
                            while (!(std::cin >> userVal)) {
                                std::cin.clear();
                                std::cin.ignore(10000, '\n');
                                std::cout << "Invalid input. Please enter a number for '" << c << "': ";
                            }
                            
                            // 2. Swap the letter for the number in the formula string
                            formulaText = replaceVariable(formulaText, c, userVal);
                        }
                    }

                    std::cout << "\n[DEBUG] String sent to engine: " << formulaText << "\n";

                    // 3. Final calculation using your Plan B parser
                    double finalResult = evaluate(formulaText);

                    // Special case for pythagorean theorem
                    if (mathList[index].name == "Pythagorean Theorem") {
                        finalResult = std::sqrt(finalResult);
                        }
                        
                    std::cout << "\n-----------------------------------------------------";
                    std::cout << "\nFinal Result: " << finalResult << std::endl;
                    std::cout << "-----------------------------------------------------\n";
                        
                    std::cin.ignore(10000, '\n'); // Clear buffer for next menu
                    break;
                    }

                    case 3:
                        inSubMenu = false;
                        system("cls");
                        break;
                    
                default:
                    std::cout << "Error: Please choose a valid option\n";
                    break;
                }
    
            }
        }
        else {
            std::cout << "Error: Please choose a valid option\n";
        }
    }
    return 0;
}