//
// Created by Preet Rana on 16/07/25.
//

#include <iostream>

#include "../includes/TextEditor.h"

int main() {

    std::string filename;
    std::cout<<"Enter file name: ";
    std::cin>>filename;
    TextEditor editor(filename);
    editor.run();

    return 0;
}
