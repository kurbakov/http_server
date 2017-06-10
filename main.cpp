#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>


const char* in_file = "/Users/dmytro.kurbakov/Desktop/dev/in_out/in.fifo";
const char* out_file = "/Users/dmytro.kurbakov/Desktop/dev/in_out/out.fifo";

int main(int argc, char const *argv[])
{
	char* rd_buffer[1024];

	if(fork())
	{
		if(mkfifo(in_file, 0666)){
			std::cout << "can not create MKFIFO IN.fifo" << "\n";
			return 0;
		}

		int in_fd = open(in_file, O_RDONLY | O_NONBLOCK);
		read(in_fd, rd_buffer, sizeof(rd_buffer));
		close(in_fd);
	}
	else
	{
		if(mkfifo(out_file, 0666)){
			std::cout << "can not create MKFIFO OUT.fifo" << "\n";
			return 0;
		}
		int out_fd = open(out_file, O_WRONLY);
		write(out_fd, rd_buffer, sizeof(rd_buffer));
		close(out_fd);
	}
	return 0;
}
