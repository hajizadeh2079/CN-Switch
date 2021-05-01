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


int main(int argc, char const *argv[]) {
    int number_of_ports = atoi(argv[0]);
    int switch_number = atoi(argv[1]);
    message_buffer msg_buff;
    int key = switch_number;
    int msgid = msgget(key, 0666 | IPC_CREAT);
    while (true) {
        if (msgrcv(msgid, &msg_buff, sizeof(msg_buff), 1, IPC_NOWAIT) != -1) {
            cout << switch_number << endl;
            cout << msg_buff.msg_text << endl;
        }
    }
    exit(0);
}