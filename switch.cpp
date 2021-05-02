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


void process_frame(vector<string> data, int *port_table, int *is_switch) {
    int sender = atoi(data[0].c_str());
    int receiver = atoi(data[1].c_str());
    if (sender == 0) {
        port_table[atoi(data[3].c_str())] = atoi(data[2].c_str());
        if (data.size() == 5)
            is_switch[atoi(data[3].c_str())] = 1;
    }
    else {
    }
}


int main(int argc, char const *argv[]) {
    int number_of_ports = atoi(argv[0]);
    int switch_number = atoi(argv[1]);
    int *port_table = (int*)malloc(number_of_ports * sizeof(int));
    int *is_switch = (int*)malloc(number_of_ports * sizeof(int));
    for (int i = 0; i < number_of_ports; i++) {
        port_table[i] = -1;
        is_switch[i] = 0;
    }
    message_buffer msg_buff;
    int key = switch_number;
    int msgid = msgget(key, 0666 | IPC_CREAT);
    while (true) {
        if (msgrcv(msgid, &msg_buff, sizeof(msg_buff), 1, IPC_NOWAIT) != -1) {
            string frame(msg_buff.msg_text);
            vector<string> data = split_frame(frame);
            process_frame(data, port_table, is_switch);
        }
    }
    exit(0);
}