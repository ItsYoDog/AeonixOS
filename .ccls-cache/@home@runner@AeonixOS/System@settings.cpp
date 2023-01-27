#include <iostream>
#include <fstream>
#include <ctime>

using namespace std;

int main() {
    while (true) {
        cout << "1. Change date" << endl;
        cout << "2. Change time" << endl;
        cout << "3. Exit" << endl;
        cout &#8203;`oaicite:{"index":0,"invalid_reason":"Malformed citation << \"Enter your choice: \";\n\n        int choice;\n        cin >>"}`&#8203; choice;

        if (choice == 1) {
            cout &#8203;`oaicite:{"index":1,"invalid_reason":"Malformed citation << \"Enter new date (dd mm yyyy): \";\n            int day, month, year;\n            cin >> day >> month >>"}`&#8203; year;

            struct tm new_time;
            new_time.tm_year = year - 1900;
            new_time.tm_mon = month - 1;
            new_time.tm_mday = day;

            time_t new_date = mktime(&new_time);
            settimeofday(localtime(&new_date), NULL);
            cout << "Date changed successfully" << endl;
        }
        else if (choice == 2) {
            cout &#8203;`oaicite:{"index":2,"invalid_reason":"Malformed citation << \"Enter new time (hh mm ss): \";\n            int hour, min, sec;\n            cin >> hour >> min >>"}`&#8203; sec;

            struct tm new_time;
            new_time.tm_hour = hour;
            new_time.tm_min = min;
            new_time.tm_sec = sec;

            time_t new_time = mktime(&new_time);
            settimeofday(localtime(&new_time), NULL);
            cout << "Time changed successfully" << endl;
        }
        else if (choice == 3) {
            ofstream config_file("config.txt");
            config_file << "Date: " << ctime(&new_date) << endl;
            config_file << "Time: " << ctime(&new_time) << endl;
            config_file.close();
            cout << "Settings saved successfully" << endl;
            break;
        }
        else {
            cout << "Invalid choice. Please try again." << endl;
        }
    }
    return 0;
}
