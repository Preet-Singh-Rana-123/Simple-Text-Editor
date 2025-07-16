//
// Created by Preet Rana on 16/07/25.
//

#ifndef TEXTEDITOR_H
#define TEXTEDITOR_H
#include <string>
#include <fstream>


class TextEditor {
    public:
    void openFile(const std::string& fileName);
    void writeToFile();
    void run();
    void displayContent();

    private:
    std::string fileName;
    std::vector<std::string> content;
};



#endif //TEXTEDITOR_H
