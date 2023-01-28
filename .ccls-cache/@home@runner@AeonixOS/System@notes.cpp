#include <iostream>
#include <fstream>
#include <string>

using namespace std;

void printMenu() {
    cout << "Aeonix Notepad" << endl;
    cout << "1. Create a new file" << endl;
    cout << "2. Open a file" << endl;
    cout << "3. Save a file" << endl;
    cout << "4. Save a file as" << endl;
    cout << "5. Close a file" << endl;
    cout << "6. Exit" << endl;
}

int notepad() {
    string input;
    string fileName;
    string fileContent;
    ofstream outFile;
    ifstream inFile;

    while (true) {
        printMenu();
        getline(cin, input);

        if (input == "1") {
            cout << "Enter a file name: ";
            getline(cin, fileName);
            outFile.open(fileName);
            cout << "File created." << endl;
        } else if (input == "2") {
            cout << "Enter a file name: ";
            getline(cin, fileName);
            inFile.open(fileName);
            if (inFile.is_open()) {
                cout << "File opened." << endl;
                while (getline(inFile, fileContent)) {
                    cout << fileContent << endl;
                }
                inFile.close();
            } else {
                cout << "Unable to open file." << endl;
            }
        } else if (input == "3") {
            if (outFile.is_open()) {
                cout << "Enter file content: ";
                getline(cin, fileContent);
                outFile << fileContent;
                outFile.close();
                cout << "File saved." << endl;
            } else {
                cout << "No file open." << endl;
            }
        } else if (input == "4") {
            cout << "Enter a file name: ";
            getline(cin, fileName);
            outFile.open(fileName);
            cout << "Enter file content: ";
            getline(cin, fileContent);
            outFile << fileContent;
            outFile.close();
            cout << "File saved." << endl;
        } else if (input == "5") {
            if (outFile.is_open()) {
                outFile.close();
                cout << "File closed." << endl;
            } else {
                cout << "No file open." << endl;
            }
        } else if (input == "6") {
            if (outFile.is_open()) {
                outFile.close();
            }
            break;
        } else {
            cout << "Invalid input." << endl;
        }
    }

    return 0;
}
