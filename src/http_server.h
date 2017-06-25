#include <string> // std::string
#include <fcntl.h> // flag FIOBIO in set_nonblock
#include <arpa/inet.h> // inet_pton()
#include <sys/epoll.h> // epoll part
//#include <sys/socket.h>
//#include <sys/types.h>
//#include <sys/stat.h>


const int MAXEVENTS = 256;

class http_server{
private:
	std::string host;
	std::string port;
	std::string directory;

	int initiate_socket(std::string h, std::string p);
	int set_nonblock(int fd);
	void handle_requests();
	
public:
	http_server(std::string, std::string, std::string);
	~http_server();

	void run();
};