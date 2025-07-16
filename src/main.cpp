//
// Created by Preet Rana on 16/07/25.
//

#include <iostream>

#include "../includes/TextEditor.h"

int main() {
    TextEditor editor;
    std::string fileName;
    std::cout<<"Enter file name to open: ";
    std::cin>>fileName;

    editor.openFile(fileName);
    std::cout<<"\n------ Editing "<<fileName<<" current content ------\n";
    editor.displayContent();
    std::cout << "Start typing (use :w to save, :q to quit, :wq to save and quit, :d to display file content):\n";
    editor.run();
    return 0;
}
