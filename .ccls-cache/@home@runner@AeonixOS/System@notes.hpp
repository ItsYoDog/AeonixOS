#ifndef NOTEPAD_HPP
#define NOTEPAD_HPP

#include <iostream>
#include <fstream>
#include <string>

class Notepad {
private:
    std::string file_name;
    std::string file_path;
    std::string file_contents;

public:
    Notepad();
    Notepad(std::string file_path);
    ~Notepad();

    void setFileName(std::string file_name);
    void setFilePath(std::string file_path);
    std::string getFileName();
    std::string getFilePath();

    void createNewFile();
    void openFile();
    void saveFile();
    void saveFileAs();
    void closeFile();
    void displayFile();
    void appendToFile();
    void insertIntoFile();
    void replaceInFile();
    void deleteFromFile();
};

#endif
