// Libraries
#include <chrono>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <dirent.h>
#include <errno.h>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <sys/stat.h>
#include <sys/statvfs.h>
#include <sys/types.h>
#include <thread>
#include <unistd.h>
#include <vector>

// Programs/built-it apps
#include "calc.hpp"
int calculator();
#include "notes.hpp"
int notepad();

// The main code itself

using namespace std;

std::string clipboardFileName;
std::string clipboardFileContent;

void printVersion() {
  cout << "Version: Build 1991 (Prototype)" << endl;
  cout << "Last updated: 28-01-2023" << endl;
  cout << "Made by: © PixelWare Studios 2023" << endl;
}

void showStartupLoadingAnimation() {
  cout << "Loading scripts";
  for (int i = 0; i < 3; i++) {
    cout << ".";
    cout.flush();
    this_thread::sleep_for(chrono::seconds(1));
  }
  cout << endl;
  cout << R"(
                           _        ____   _____ 
     /\                   (_)      / __ \ / ____|
    /  \   ___  ___  _ __  ___  __| |  | | (___  
   / /\ \ / _ \/ _ \| '_ \| \ \/ /| |  | |\___ \ 
  / ____ \  __/ (_) | | | | |>  < | |__| |____) |
 /_/    \_\___|\___/|_| |_|_/_/\_(_)____/|_____/ 
                                                 
                                                 
  )" << endl;
}

void printDateTime() {
  time_t now = time(0);
  char *dt = ctime(&now);
  cout << "Date and time: " << dt << endl;
}

void checkDiscUsage() {
  // Use system commands to get the total storage and used storage
  system("df -h > storage.txt");
  ifstream storageFile("storage.txt");

  // Print a fancy header
  cout << "\033[1;32m========== Storage Information ==========\033[0m" << endl;

  // Read the storage information from the file and print it out
  string line;
  while (getline(storageFile, line)) {
    // Use stringstream to tokenize the line
    stringstream ss(line);
    vector<string> tokens;
    string token;
    while (getline(ss, token, ' ')) {
      // Add non-empty tokens to the vector
      if (!token.empty()) {
        tokens.push_back(token);
      }
    }

    // Print out the information with consistent left alignment
    cout << setw(15) << left << tokens[0] << setw(15) << left << tokens[1]
         << setw(15) << left << tokens[2] << setw(15) << left << tokens[3]
         << tokens[4] << endl;
  }

  // Remove the storage file
  storageFile.close();
  remove("storage.txt");
}

void listFiles() {
  DIR *dir;
  struct dirent *ent;
  struct stat statbuf;
  vector<pair<string, int>> files;
  long total_size = 0;
  int max_length = 0;
  if ((dir = opendir(".")) != NULL) {
    while ((ent = readdir(dir)) != NULL) {
      if (ent->d_type == DT_REG) {
        files.push_back({ent->d_name, ent->d_type});
        stat(ent->d_name, &statbuf);
        total_size += statbuf.st_size;
        max_length = max(max_length, (int)strlen(ent->d_name));
      } else if (ent->d_type == DT_DIR) {
        if (strcmp(ent->d_name, "System") == 0) {
          files.insert(files.begin(),
                       {"\033[1;31m<SYS> " + string(ent->d_name) + "\033[0m",
                        ent->d_type});
        } else {
          files.insert(files.begin(), {ent->d_name, ent->d_type});
        }
      }
    }
    closedir(dir);
    for (auto file : files) {
      if (file.first[0] == '.')
        continue;
      if (file.second == DT_DIR)
        cout << "<DIR> ";
      else {
        stat(file.first.c_str(), &statbuf);
        cout << setw(8) << statbuf.st_size << " bytes | ";
      }
      cout << setw(max_length) << file.first << endl;
    }
    cout << "Total storage used: " << total_size << " bytes" << endl;
  } else {
    cout << "Error: Unable to open directory." << endl;
  }
}

void deleteFileOrDirectory(string path) {
  int result = remove(path.c_str());
  if (result == 0) {
    cout << "File/Directory " << path << " deleted successfully." << endl;
  } else {
    cout << "Error: Unable to delete file/directory " << path << "." << endl;
  }
}

void executeSysFile(string file_name) {
  std::string output_file = file_name.substr(
      0, file_name.length() - 4); // remove ".cpp" from file name
  std::string command_line = "g++ " + file_name + " -o " + output_file;
  system(command_line.c_str());
  command_line = "./" + output_file;
  system(command_line.c_str());
}

void copyFile(std::string fileName) {
  std::ifstream sourceFile(fileName);
  if (sourceFile.is_open()) {
    clipboardFileContent =
        std::string((std::istreambuf_iterator<char>(sourceFile)),
                    std::istreambuf_iterator<char>());
    sourceFile.close();
    clipboardFileName = fileName;
    std::cout << "File '" << fileName << "' copied to clipboard." << std::endl;
  } else {
    std::cout << "Error opening file '" << fileName << "'." << std::endl;
  }
}

void pasteFile(std::string fileName) {
  if (clipboardFileContent.empty()) {
    std::cout << "Clipboard is empty." << std::endl;
    return;
  }
  std::ofstream destinationFile(fileName);
  if (destinationFile.is_open()) {
    destinationFile << clipboardFileContent;
    destinationFile.close();
    std::cout << "File '" << clipboardFileName << "' pasted as '" << fileName
              << "'." << std::endl;
  } else {
    std::cout << "Error creating file '" << fileName << "'." << std::endl;
  }
}

// find command

bool warningDisplayed = false;

void searchDisc(string directory, string fileName) {
  DIR *dir;
  struct dirent *ent;
  int total = 0;
  int found = 0;

  if (!warningDisplayed) {
    cout << "\033[1;31mThis may take a while.\033[0m" << endl;
    cout << "Press 'q' to exit" << endl;
    warningDisplayed = true;
  }
  if ((dir = opendir(directory.c_str())) != NULL) {
    while ((ent = readdir(dir)) != NULL) {
      total++;
    }
    closedir(dir);
  } else {
    cout << "Could not open directory" << endl;
    return;
  }
  if ((dir = opendir(directory.c_str())) != NULL) {
    while ((ent = readdir(dir)) != NULL) {
      string fname = ent->d_name;
      if (fname == "." || fname == "..") {
        continue;
      }
      if (ent->d_type == DT_DIR) {
        // directory
        string newDir = directory + '/' + fname;
        searchDisc(newDir, fileName);
      } else if (ent->d_type == DT_REG) {
        // file
        if (fname == fileName) {
          cout << "File found at: " << directory << '/' << fname << endl;
          found++;
        }
      }
      if (cin.get() == 'q') {
        cout << "Search stopped." << endl;
        closedir(dir);
        return;
      }
      cout << "\r"
           << "Progress: " << found << "/" << total << " files searched";
      cout.flush();
    }
    closedir(dir);
  } else {
    cout << "Could not open directory" << endl;
  }
}

int find() {
  string fileName;
  cout << "Enter the name of the file or directory you want to search for: ";
  getline(cin, fileName);
  cout << " " << endl;
  cout << "\033[1;32mSearching for " << fileName
       << " on the entire disc...\033[0m" << endl;
  searchDisc("/", fileName);
  return 0;
}

// Help menu

void printCommands(vector<pair<string, string>> commands) {
  int max_length = 0;
  for (auto command : commands) {
    max_length = max(max_length, (int)command.first.length());
  }
  for (auto command : commands) {
    cout << setw(max_length) << left << command.first << " | " << command.second
         << endl;
  }
}

void printFileCommands() {
  cout << "File management commands:" << endl;
  vector<pair<string, string>> commands = {
      {"write", "write a file"},
      {"read", "read a file"},
      {"open <file_name>", "open a file"},
      {"sys <file_name>", "run code from a sys file"},
      {"ls", "display the files in the current directory"},
      {"mkdir <folder_name>", "create a new folder"},
      {"cd <folder_name>", "open a folder"},
      {"rm", "delete a file or folder"},
      {"cf", "Copies a file"},
      {"pf", "Pastes the file in the clipboard"}};
  printCommands(commands);
}

void printSystemCommands() {
  cout << "System commands:" << endl;
  vector<pair<string, string>> commands = {
      {"date", "display the current date and time"},
      {"version", "display the current version"},
      {"help", "display a list of commands"},
      {"exit", "exit the program"},
      {"cls", "Clears the screen"},
      {"disc", "Shows advanced disc information"},
      {"find", "Searches disc for specific file"},
  };
  printCommands(commands);
}

void printProgramCommands() {
  cout << "Program commands:" << endl;
  vector<pair<string, string>> commands = {
      {"calc", "Runs the built-in calculator program"},
      {"notes", "Runs the built-in notepad program"},
  };
  printCommands(commands);
}

void printHelp() {
  int choice;
  while (true) {
    cout << "\033[0;34mSelect a category:\033[0m" << endl;
    cout << "1. File management" << endl;
    cout << "2. System commands" << endl;
    cout << "3. Program commands" << endl;
    cout << "4. Exit" << endl;
    cout << "help > ";
    string input;
    getline(cin, input);
    choice = atoi(input.c_str());
    switch (choice) {
    case 1:
      printFileCommands();
      break;
    case 2:
      printSystemCommands();
      break;
    case 3:
      printProgramCommands();
      break;
    case 4:
      return;
      break;
    default:
      cout << "Invalid choice." << endl;
      break;
    }
  }
}

// Main CLI interface
int main() {
  size_t found;
  string command;
  string file_name;
  string file_content;

  chdir("user");

  cout << "Welcome to the Aeonix.OS remote CLI system" << endl;
  showStartupLoadingAnimation();
  printVersion();
  cout << " " << endl;
  printDateTime();
  while (true) {
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    cout << "\033[34m" << cwd << " > \033[0m";
    getline(cin, command);

    if (command == "cf") {
      std::string fileName;
      std::cout << "Enter filename: " << fileName;
      getline(cin, fileName);
      copyFile(fileName);
    } else if (command == "pf") {
      std::string fileName;
      std::cout << "Enter filename: " << fileName;
      getline(cin, fileName);
      pasteFile(fileName);
    } else if (command == "write") {
      cout << "Enter file name: ";
      getline(cin, file_name);
      cout << "Enter file content: ";
      getline(cin, file_content);
      ofstream out_file;
      out_file.open(file_name);
      out_file << file_content;
      out_file.close();
      cout << "File written successfully." << endl;
    } else if (command == "calc") {
      calculator();
      cout << " " << endl;
      main();
    } else if (command == "notes") {
      notepad();
      cout << " " << endl;
      main();
    } else if (command == "find") {
      find();
    } else if (command == "disc") {
      checkDiscUsage();
    } else if (command == "read") {
      cout << "Enter file name: ";
      getline(cin, file_name);
      ifstream in_file;
      in_file.open(file_name);
      if (in_file.is_open()) {
        while (getline(in_file, file_content)) {
          cout << file_content << endl;
        }
        in_file.close();
      } else {
        cout << "Error: Unable to open file." << endl;
      }
    } else if (command == "open") {
      cout << "Enter file name: ";
      getline(cin, file_name);
      ifstream in_file;
      in_file.open(file_name);
      if (in_file.is_open()) {
        while (getline(in_file, file_content)) {
          cout << file_content << endl;
        }
        in_file.close();
      } else {
        cout << "Error: Unable to open file." << endl;
      }
    } else if (command.substr(0, 3) == "sys") {
      file_name = command.substr(4);
      executeSysFile(file_name);
    } else if (command == "date") {
      printDateTime();
    } else if (command == "version") {
      printVersion();
    } else if (command == "ls") {
      listFiles();
    } else if (command.substr(0, 5) == "mkdir") {
      file_name = command.substr(6);
      if (file_name != "") {
        string command = "mkdir " + file_name;
        system(command.c_str());
        cout << "Folder created successfully." << endl;
      } else {
        cout << "Error: Please enter a valid folder name." << endl;
      }
    } else if (command.substr(0, 2) == "cd") {
      file_name = command.substr(3);
      if (file_name != "") {
        if (file_name == "System") {
          cout << "\033[1;31m WARNING: Accessing system directory can be "
                  "harmful to your computer. Only continue in this directory "
                  "if you know what you are doing!\033[0m"
               << endl;
        }
        if (chdir(file_name.c_str()) == 0) {
          cout << "Changed working directory to " << file_name << endl;
        } else {
          cout << "Error: Unable to change working directory" << endl;
        }
      } else {
        cout << "Error: Please enter a valid folder name." << endl;
      }
    } else if (command == "help") {
      printHelp();
    } else if (command == "exit") {
      cout << "Shutting down..." << endl;
      break;
    } else if (command == "cls") {
      system("clear");
    } else if (command == "rm") {
      cout << "Enter file/folder name: ";
      string path;
      getline(cin, path);
      deleteFileOrDirectory(path);
    } else {
      cout << "Error: Invalid command." << endl;
    }
  }
  return 0;
}