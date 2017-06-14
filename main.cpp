#include <iostream>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <fstream>

struct message {
    long mtype;
    char mtext[80];
};

int main()
{
    // get the key
    key_t key = ftok("/tmp/msg.temp", 1);

    // create queue
    int queue_code = msgget(key, IPC_CREAT|IPC_EXCL|0666);

    // create file
    std::ofstream output_file("/home/box/message.txt");

//    if(fork()) {
//        message msg;
//        std::strcpy(msg.mtext, "my message is here!");
//        msg.mtype = 1;
//
//        ssize_t rc = msgsnd(queue_code, &msg, sizeof(msg.mtext), 0);
//        if (rc < 0) {
//            std::cout << "msgsnd failed, rc = " << rc << "\n";
//            return 1;
//        }
//    }
//    sleep(1);

    if(fork()){
        message msg;
        ssize_t rc = msgrcv(queue_code, (void *) &msg, sizeof(msg.mtext), 0, 0);

        if (rc < 0) {
            std::cout << "msgrcv failed, rc = " << rc << "\n";
            return 1;
        }
        std::cout << msg.mtext << "\n";
    }

    // remove queue
//    return_code = msgctl(queue_code, IPC_RMID, NULL);
//    if (return_code < 0) {
//        perror( strerror(errno) );
//        printf("msgctl (return queue) failed, rc=%d\n", return_code);
//        return 1;
//    }
//    printf("message queue %d is gone\n", return_code);

    return 0;
}
