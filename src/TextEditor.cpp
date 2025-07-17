//
// Created by Preet Rana on 16/07/25.
//

#include "../includes/TextEditor.h"

#include <iostream>

// Constructors and destructors

TextEditor::TextEditor(const std::string &file) {
    this->fileName = file;
    lines.push_back("");
    loadFile();
}

// Public Function

void TextEditor::run() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(1);

    while (running) {
        clear();
        drawText();
        move(cursor.y, cursor.x);
        refresh();

        int ch = getch();
        handleInput(ch);
    }
    endwin();
}

// Private Functions

void TextEditor::handleInput(int ch) {
    switch (ch) {
        case 7:
            saveFile();
            break;
        case 24:
            running = false;
            break;
        case KEY_UP:
            if (cursor.y > 0)
                cursor.y--;
            cursor.x = std::min(cursor.x, (int)lines[cursor.y].length());
            break;
        case KEY_DOWN:
            if (cursor.y < (int)lines.size() - 1)
                cursor.y++;
            cursor.x = std::min(cursor.x, (int)lines[cursor.y].length());
            break;
        case KEY_LEFT:
            if (cursor.x > 0)
                cursor.x--;
            break;
        case KEY_RIGHT:
            if (cursor.x < (int)lines[cursor.y].length())
                cursor.x++;
            break;
        case 10:
            insertNewLine();
            break;
        case KEY_BACKSPACE:
        case 127:
            deleteChar();
            break;
        default:
            if (isprint(ch)) {
                insertChar((char)ch);
            }
            break;
    }
}


// function for writing test on window on the correct positions
void TextEditor::drawText() {
    for (size_t i = 0; i < lines.size(); ++i) {
        mvprintw(i, 0, "%s", lines[i].c_str());
    }
}

// function for the status bar which show fileName, no. of lines and columns
void TextEditor::drawStatusBar() {
    int rows, cols;
    getmaxyx(stdscr, rows, cols);  // store rows and cols of standard screen
    attron(A_REVERSE);  // reverse the color of background and foreground
    mvprintw(rows - 1, 0, "File: %s | Ln: %d, col: %d | Ctrl+G: Save | Ctrl+X: Exit", fileName.c_str(), cursor.x + 1,
             cursor.y + 1);
    attroff(A_REVERSE); // turn off the attron
}

// function for insering character in lines vector
void TextEditor::insertChar(char c) {
    lines[cursor.y].insert(cursor.x,1,c);
    cursor.x++;
}

// function for inserting characters in new line whenever user press enter
void TextEditor::insertNewLine() {
    std::string newLine = lines[cursor.y].substr(cursor.x);
    lines[cursor.y] = lines[cursor.y].substr(0,cursor.x);
    lines.insert(lines.begin() + cursor.y + 1, newLine);
    cursor.x = 0;
    cursor.y++;
}

// function for deleting character when backspace pressed
void TextEditor::deleteChar() {
    if (cursor.x > 0) {
        // this condition for normal deletion when cursor is at last or between letters
        lines[cursor.y].erase(cursor.x-1,1);
        cursor.x--;
    }else if (cursor.y > 0) {
        // this condition when cursor is at the starting of the line even before first letter
        // here we have to merge that line with above line
        cursor.x = lines[cursor.y - 1].length();
        lines[cursor.y - 1] += lines[cursor.y];
        lines.erase(lines.begin() + cursor.y);
        cursor.y--;
    }
}

// function for loading all data of file into our lines vector so that we can perform operation on them
void TextEditor::loadFile() {
    std::ifstream inFile(fileName);
    if (!inFile) return;
    lines.clear();
    std::string line;
    while (std::getline(inFile,line)) {
        lines.push_back(line);
    }
    if (lines.empty()) {
        lines.push_back("");
    }
}

// function for saving the data that we have added in our text editor
void TextEditor::saveFile() {
    std::ofstream outFile(fileName);
    if (!outFile) return;
    for (auto& line : lines) {
        outFile << line << "\n";
    }
}
