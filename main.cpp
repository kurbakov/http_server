#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <fstream>

struct message {
    long mtype;
    char mtext[80];
};

int main()
{
    // get the key
    int key = ftok("/tmp/msg.temp", 1);
    std::cout << key << "\n";

    // create queue
    int queue_code = msgget(key, IPC_CREAT|IPC_EXCL|0666);

    // create file
    std::ofstream output_file("/home/box/message.txt");

//    if(fork()) {
//        message msg_send;
//        std::strcpy(msg_send.mtext, "my message is here!");
//        msg_send.mtype = 1;
//
//        ssize_t rc = msgsnd(queue_code, &msg_send, sizeof(msg_send.mtext), 0);
//        if (rc < 0) {
//            perror( strerror(errno) );
//            std::cout << "msgsnd failed, rc = " << rc << "\n";
//            return 1;
//        }
//    }
//
    if(fork()) {
        message msg_recv;
        ssize_t rc = msgrcv(queue_code, &msg_recv, sizeof(msg_recv.mtext), 0, 0);

        if (rc < 0) {
            perror(strerror(errno));
            std::cout << "msgrcv failed, rc = " << rc << "\n";
            return 1;
        }
        output_file << msg_recv.mtext;
        std::cout << msg_recv.mtext << "\n";
    }

    return 0;
}
