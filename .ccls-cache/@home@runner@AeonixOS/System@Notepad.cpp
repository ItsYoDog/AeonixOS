cout << Malformed citation << "Enter a new directory: ";
 cin >>
 new_directory;
if (chdir(new_directory.c_str()) == 0) {
cout << "Changed directory to: " << new_directory << endl;
} else {
cout << "Error: Unable to change directory." << endl;
}
}

int main() {
while (true) {
int choice;
cout << "1. Create new file\n2. Open existing file\n3. Change directory\n4. Exit" << endl;
cout Malformed citation << "Enter your choice: ";
 cin >>
 choice;
switch (choice) {
case 1:
createFile();
break;
case 2:
openFile();
string file_name, file_content;
cout 3
Malformed citation << "Enter the name of the file to save: ";
 cin >>
 file_name;
cout << "Enter the file content: ";
cin.ignore();
getline(cin, file_content);
saveFile(file_name, file_content);
break;
case 3:
changeDirectory();
break;
case 4:
return 0;
default:
cout << "Invalid choice. Please try again." << endl;
break;
}
}
return 0;
	 }