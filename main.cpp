#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <mqueue.h>
#include <iostream>
#include <fstream>

#define MSG_SIZE 4096

int main(int argc, char **argv)
{
    char buffer[MSG_SIZE + 1];

    // initialize the queue attributes
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = MSG_SIZE;
    attr.mq_curmsgs = 0;

    // create file
    std::ofstream output_file("/home/box/message.txt");

    /* create the message queue */
    mqd_t mq = mq_open("/test.mq", O_CREAT | O_RDWR, 0666, &attr);

    if(fork()){
        ssize_t bytes_read = mq_receive(mq, buffer, MSG_SIZE, NULL);

        buffer[bytes_read] = '\0';
        output_file << buffer;
	}

    // mq_close(mq);

    return 0;
}
