//
// Created by Preet Rana on 16/07/25.
//

#include "TextEditor.h"

#include <iostream>

void TextEditor::run() {
    while (true) {
        std::string input;
        std::getline(std::cin, input);
        if (input == ":q") {
            std::cout<<"[Exiting editor]\n";
            break;
        }else if (input == ":w") {
            writeToFile();
        }else if (input == ":wq") {
            writeToFile();
            break;
        }else if (input == ":d") {
            displayContent();
        }else {
            content.push_back(input);
        }
    }
}


void TextEditor::openFile(const std::string& name) {
    fileName = name;
    std::ifstream inFile(fileName);
    if (!inFile.is_open()) {
        std::ofstream createFile(fileName);
        if (!createFile.is_open()) {
            std::cerr << "Error creating file " << fileName << "\n";
            return;
        }
        createFile.close();
        std::cout << "[New file created: " << fileName << "]\n";
        return;
    }

    std::string line;
    while (std::getline(inFile, line)) {
        content.push_back(line);
    }

    inFile.close();
}

void TextEditor::writeToFile() {
    std::ofstream outFile(fileName, std::ios::trunc);
    for (const auto& c : content) {
        outFile << c << "\n";
    }
    outFile.close();
    std::cout<<"[File saved]\n";
}

void TextEditor::displayContent() {
    for (const auto& line : content) {
        std::cout << line << "\n";
    }
}

