#include <iostream>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>

struct message {
    long mtype;
    char mtext[80];
} msg;

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
    // get the key
    key_t key = ftok("/tmp/msg.temp", 1);

    // create queue
    int qc = create__queue(key);

    if(fork()){
        ssize_t rc = msgrcv(qc, &msg, sizeof(msg.mtext), 0, 0);
        if (rc < 0) {
            perror( strerror(errno) );
            printf("msgrcv failed, rc=%d\n", rc);
            return 1;
        }
        printf("received msg: %s\n", msg.mtext);

        // pause();
    }

    // remove queue
//    int rc = msgctl(qc, IPC_RMID, NULL);
//    if (rc < 0) {
//        perror( strerror(errno) );
//        printf("msgctl (return queue) failed, rc=%d\n", rc);
//        return 1;
//    }
//    printf("message queue %d is gone\n", qc);

    return 0;
}
