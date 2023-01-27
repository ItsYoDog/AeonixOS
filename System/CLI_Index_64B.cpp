#include <chrono>
#include <ctime>
#include <dirent.h>
#include <errno.h>
#include <fstream>
#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <thread>
#include <unistd.h>
#include <vector>
#include <iomanip>
#include <cstring>

// Programs / built-it apps
#include "calc.hpp"
int calculator();

using namespace std; 

void printVersion() {
	cout << "Version: Build 287 (Prototype)" << endl;
	cout << "Last updated: 26-01-2023" << endl;
	cout << "Made by: © PixelWare Studios 2023" << endl;
}

void showStartupLoadingAnimation() {
  cout << "Loading scripts";
  for (int i = 0; i < 2; i++) {
    cout << ".";
    cout.flush();
    this_thread::sleep_for(chrono::seconds(1));
  }
  cout << endl;
}

void printDateTime() {
  time_t now = time(0);
  char *dt = ctime(&now);
  cout << "Date and time: " << dt << endl;
}

void printHelp() {
  int max_length = 0;
  cout << "Commands:" << endl;
  vector<pair<string, string>> commands = {
      {"write", "write a file"},
      {"read", "read a file"},
      {"open <file_name>", "open a file"},
      {"sys <file_name>", "run code from a sys file"},
      {"date", "display the current date and time"},
      {"version", "display the current version"},
      {"ls", "display the files in the current directory"},
      {"mkdir <folder_name>", "create a new folder"},
      {"cd <folder_name>", "open a folder"},
      {"rm", "delete a file or folder"},
      {"help", "display a list of commands"},
      {"exit", "exit the program"},
	{"clear", "Clears the screen"},
  };
  for (auto command : commands) {
    max_length = max(max_length, (int)command.first.length());
  }
  for (auto command : commands) {
    cout << setw(max_length) << left << command.first << " | " << command.second << endl;
  }
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
                files.insert(files.begin(), {ent->d_name, ent->d_type});
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
    std::string output_file = file_name.substr(0, file_name.length() - 4); // remove ".cpp" from file name
    std::string command_line = "g++ " + file_name + " -o " + output_file;
    system(command_line.c_str());
    command_line = "./" + output_file;
    system(command_line.c_str());
}


int main() {
  string command;
  string file_name;
  string file_content;

  cout << "Welcome to the Aeonix.OS remote CLI system" << endl;
  showStartupLoadingAnimation();
  printVersion();
	cout << " " << endl;
  printDateTime();
  while (true) {
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    cout << cwd << " > ";
    getline(cin, command);

    if (command == "write") {
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
          cout << "\033[1;31m WARNING: Accessing system directory can be harmful to your computer. Only continue in this directory if you know what you are doing!\033[0m" << endl;
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
    } else if (command == "clear") {
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