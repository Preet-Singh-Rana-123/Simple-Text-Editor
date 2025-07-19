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
        drawStatusBar();
        move(cursor.y, cursor.x + lineNumberOffset);
        refresh();

        int ch = getch();
        handleInput(ch);
    }
    endwin();
}

// Private Functions

void TextEditor::handleInput(int ch) {
    int rows, cols;
    getmaxyx(stdscr, rows, cols);

    int maxVisibleRows = rows - 1;

    if (cursor.y < topLineIndex) {
        topLineIndex = cursor.y;
    }
    if (cursor.y >= topLineIndex + maxVisibleRows) {
        topLineIndex = cursor.y - maxVisibleRows + 1;
    }

    // Make sure we never scroll past the bottom of the file
    if (topLineIndex > (int)lines.size() - maxVisibleRows) {
        topLineIndex = std::max(0, (int)lines.size() - maxVisibleRows);
    }

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
            modified = true;
            break;
        case KEY_BACKSPACE:
        case 127:
            deleteChar();
            modified = true;
            break;
        case 9:
            lines[cursor.y].insert(cursor.x,"    ");
            cursor.x += 4;
            modified = true;
            break;
        case 6:
            search();
            break;
        case 1:
            helpWindow();
            break;
        default:
            if (isprint(ch)) {
                insertChar((char)ch);
            }
            modified = true;
            break;
    }
}


// function for writing text on window on the correct positions
void TextEditor::drawText() {
    int rows, cols;
    getmaxyx(stdscr, rows, cols);
    int maxVisibleRows = rows - 1;
    int cursorScreenY = cursor.y - topLineIndex;
    int cursorScreenX = cursor.x + lineNumberOffset;

    if (cursorScreenY >= 0 && cursorScreenY < maxVisibleRows) {
        move(cursorScreenY, cursorScreenX);
    }
    for (size_t i = 0; i < maxVisibleRows &&  i + topLineIndex < lines.size(); ++i) {
        mvprintw(i, 0, "~ %s", lines[i + topLineIndex].c_str());
    }
}

// function for the status bar which show fileName, no. of lines and columns
void TextEditor::drawStatusBar() {
    int rows, cols;
    getmaxyx(stdscr, rows, cols);  // store rows and cols of standard screen
    attron(A_REVERSE);  // reverse the color of background and foreground
    if (modified) {
        mvprintw(rows - 1, 0, "File: %s [*] | Ln: %d, col: %d | Ctrl+G: Save | Ctrl+X: Exit | Ctrl+F: Search", this->fileName.c_str(), cursor.x + 1,
             cursor.y + 1);
    }else {
        mvprintw(rows - 1, 0, "File: %s | Ln: %d, col: %d | Ctrl+G: Save | Ctrl+X: Exit | Ctrl+F: Search", this->fileName.c_str(), cursor.x + 1,
             cursor.y + 1);
    }

    clrtoeol();
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

void TextEditor::search() {
    int rows, cols;
    getmaxyx(stdscr, rows, cols);
    echo();
    curs_set(1);
    mvprintw(rows - 1, 0, "Search: ");
    clrtoeol();
    char searchQueary[256];
    getstr(searchQueary);
    noecho();

    for (int i = cursor.y; i < lines.size(); ++i) {
        size_t pos = lines[i].find(searchQueary);
        if (pos != std::string::npos) {
            cursor.x = pos;
            cursor.y = i;
            break;
        }
    }
}

void TextEditor::helpWindow() {
    WINDOW* helpWindow = newwin(10,40,5,10);
    box(helpWindow,0,0);
    mvwprintw(helpWindow, 1, 2, "Ctrl+X: Exit");
    mvwprintw(helpWindow, 2, 2, "Ctrl+G: Save");
    mvwprintw(helpWindow, 3, 2, "Ctrl+F: Search");
    mvwprintw(helpWindow, 4, 2, "Ctrl+H: Help");
    mvwprintw(helpWindow, 5, 2, "Press any key to close...");
    wrefresh(helpWindow);
    getch();
    delwin(helpWindow);
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
