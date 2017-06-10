#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>

int main(int argc, char const *argv[])
{
	// create the pair of sockets
	int sp_fd[2];
	int sp_status = socketpair(AF_UNIX, SOCK_STREAM, 0, sp_fd);

	if(!fork()){
		int return_code;
		char buffer[80];
		char *cp;

		return_code = write(sp_fd[1], cp = "Hello?",6);
		return_code = read(sp_fd[1], buffer, sizeof buffer);
		buffer[return_code] = 0; /* NUL terminate */
  		printf("Received message '%s' from socket s[1]\n", buffer);

  		pause();
	}
	else{
		int return_code;
		char buffer[80];
		char *cp;

		return_code = read(sp_fd[0], buffer, sizeof buffer);
		buffer[return_code] = 0; /* NUL terminate */
		printf("Received message '%s' from socket s[0]\n", buffer);

		return_code = write(sp_fd[0], cp = "Go away!", 8);
		printf("Wrote message '%s' on s[0]\n", cp);

		pause();
	}

  	close(sp_fd[0]);
  	close(sp_fd[1]);

	return 0;
}
