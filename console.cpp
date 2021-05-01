#include <bits/stdc++.h>

using namespace std;


class Console {
public:
    void run() {
        string str;
        while (true) {
            getline(cin, str);
            cmd = convert_string_to_vector(str);
            process_cmd();
        }
    }

    void process_cmd() {
        if (cmd[0] == "MySwitch")
            cout << "MySwitch" << endl;
        if (cmd[0] == "MySystem")
            cout << "MySystem" << endl;
    }

    vector<string> convert_string_to_vector(string str) {
        vector<string> temp;
        istringstream ss(str);
        string word;
        while(ss >> word)
            temp.push_back(word);
        return temp;
    }
private:
    vector<string> cmd;
};


int main(int argc, char const *argv[]) {
    Console console;
    console.run();
    return 0;
}