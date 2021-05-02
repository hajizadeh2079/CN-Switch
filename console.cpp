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
            create_switch(atoi(cmd[1].c_str()), atoi(cmd[2].c_str()));
        if (cmd[0] == "MySystem")
            create_system(atoi(cmd[1].c_str()));
        if (cmd[0] == "Connect")
            connect_system_switch(atoi(cmd[1].c_str()), atoi(cmd[2].c_str()), atoi(cmd[3].c_str()));
        if (cmd[0] == "Connect-S")
            connect_switch_switch(atoi(cmd[1].c_str()), atoi(cmd[2].c_str()), atoi(cmd[3].c_str()), atoi(cmd[4].c_str()));
        if (cmd[0] == "Send") {
            string data = "";
            for (int i = 3; i < cmd.size(); i++) {
                data += cmd[i];
                if (i != cmd.size() - 1)
                    data += " ";
            }
            send_system_system(atoi(cmd[1].c_str()), atoi(cmd[2].c_str()), data);
        }
    }

    void send_system_system(int sender, int receiver, string data) {
        message_buffer msg_buff;
        int key = sender + 1000;
        int msgid = msgget(key, 0666 | IPC_CREAT);
        msg_buff.msg_type = 1;
        string msg = to_string(sender) + ':' + to_string(receiver) + ':' + data;
        strcpy(msg_buff.msg_text, msg.c_str());
        msgsnd(msgid, &msg_buff, sizeof(msg_buff), 0);
    }

    void connect_system_switch(int system_number, int switch_number, int port_number) {
        message_buffer msg_buff;
        
        int key = system_number + 1000;
        int msgid = msgget(key, 0666 | IPC_CREAT);
        msg_buff.msg_type = 1;
        string msg = "0:" + to_string(system_number) + ':' + to_string(switch_number);
        strcpy(msg_buff.msg_text, msg.c_str());
        msgsnd(msgid, &msg_buff, sizeof(msg_buff), 0);

        key = switch_number;
        msgid = msgget(key, 0666 | IPC_CREAT);
        msg_buff.msg_type = 1;
        msg = "0:" + to_string(switch_number) + ':' + to_string(system_number) + ':' + to_string(port_number);
        strcpy(msg_buff.msg_text, msg.c_str());
        msgsnd(msgid, &msg_buff, sizeof(msg_buff), 0);
    }

    void connect_switch_switch(int switch_number1, int port_number1, int switch_number2, int port_number2) {
        message_buffer msg_buff;
        
        int key = switch_number1;
        int msgid = msgget(key, 0666 | IPC_CREAT);
        msg_buff.msg_type = 1;
        string msg = "0:" + to_string(switch_number1) + ':' + to_string(switch_number2) + ':' + to_string(port_number1) + ":Sw";
        strcpy(msg_buff.msg_text, msg.c_str());
        msgsnd(msgid, &msg_buff, sizeof(msg_buff), 0);

        key = switch_number2;
        msgid = msgget(key, 0666 | IPC_CREAT);
        msg_buff.msg_type = 1;
        msg = "0:" + to_string(switch_number2) + ':' + to_string(switch_number1) + ':' + to_string(port_number2) + ":Sw";
        strcpy(msg_buff.msg_text, msg.c_str());
        msgsnd(msgid, &msg_buff, sizeof(msg_buff), 0);
    }

    void create_switch(int number_of_ports, int switch_number) {
        if (fork() == 0) {
            char num_of_ports[16], switch_no[16];
            sprintf(num_of_ports, "%d", number_of_ports);
            sprintf(switch_no, "%d", switch_number);
            char *args[]={num_of_ports, switch_no, NULL};
            execvp("./switch.out", args);
        }
    }

    void create_system(int system_number) {
        if (fork() == 0) {
            char sys_no[16];
            sprintf(sys_no, "%d", system_number);
            char *args[]={sys_no, NULL};
            execvp("./system.out", args);
        }
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


int main(int argc, char *argv[]) {
    system("ipcrm --all=msg");
    Console console;
    console.run();
    return 0;
}