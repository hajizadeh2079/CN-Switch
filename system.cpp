#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>
#include <dirent.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;


typedef struct message_buffer {
    long msg_type;
    char msg_text[1024];
} message_buffer;


vector<string> split_frame(string str) {
    vector<string> v;
    stringstream ss(str);
    while (ss.good()) {
        string substr;
        getline(ss, substr, ':');
        v.push_back(substr);
    }
    return v;
}


int main(int argc, char const *argv[]) {
    int switch_number = -1;
    int system_number = atoi(argv[0]);
    message_buffer msg_buff;
    int key = system_number + 1000;
    int msgid = msgget(key, 0666 | IPC_CREAT);
    while (true) {
        if (msgrcv(msgid, &msg_buff, sizeof(msg_buff), 1, IPC_NOWAIT) != -1) {
            string frame(msg_buff.msg_text);
            vector<string> data = split_frame(frame);
            int sender = atoi(data[0].c_str());
            int receiver = atoi(data[1].c_str());
            if (sender == 0)
                switch_number = atoi(data[2].c_str());
            else {
                if (sender == system_number) {
                    int key2 = switch_number;
                    int msgid2 = msgget(key2, 0666 | IPC_CREAT);
                    msgsnd(msgid2, &msg_buff, sizeof(msg_buff), 0);
                    cout << system_number << " sends " << data[2] << " to " << receiver << endl;
                }
                if (receiver == system_number)
                    cout << system_number << " receives " << data[2] << " from " << sender << endl;
            }
        }
    }
    exit(0);
}