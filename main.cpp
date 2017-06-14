#include <iostream>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <fstream>

struct msgbuf {
    long mtype;
    char mtext[80];
};

int create__queue(key_t key_value)
{
    int qid;

    if((qid = msgget(key_value, IPC_CREAT | 0660 )) == -1)
    {
        return(-1);
    }

    return(qid);
}

int main()
{
    int return_code;
    int queue_code;

    struct msgbuf msg;

    // get the key
    key_t key = ftok("/tmp/msg.temp", 1);

    // create queue
    queue_code = create__queue(key);

    // create file
    std::ofstream output_file("/home/box/message.txt");

    return_code = msgsnd(queue_code, (void *) &msg, sizeof(msg.mtext), 0);
    if (return_code < 0) {
        std::cout << "msgsnd failed, rc = " << return_code << "\n";
        return 1;
    }

    if(fork()){
        ssize_t rc = msgrcv(queue_code, (void *) &msg, sizeof(msg.mtext), 0, 0);
        if (rc < 0) {
            std::cout << "msgrcv failed, rc = " << return_code << "\n";
            return 1;
        }
        output_file << msg.mtext;
        
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
