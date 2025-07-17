//
// Created by Preet Rana on 16/07/25.
//

#ifndef TEXTEDITOR_H
#define TEXTEDITOR_H

#include <fstream>
#include <ncurses.h>
#include <string>
#include <vector>

struct Cursor {
    int x = 0;
    int y = 0;
};

class TextEditor {
private:
    std::vector<std::string> lines;
    Cursor cursor;
    std::string fileName;
    bool running = true;

private:
    void drawText();
    void drawStatusBar();
    void handleInput(int ch);
    void insertChar(char c);
    void deleteChar();
    void insertNewLine();
    void loadFile();
    void saveFile();

public:
    TextEditor(const std::string& fileName);
    void run();
};



#endif //TEXTEDITOR_H
