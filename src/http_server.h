#include <iostream> // cout
#include <string> // std::string, stoi
#include <fcntl.h> // flag FIOBIO in set_nonblock
#include <arpa/inet.h> // inet_pton()
#include <sys/epoll.h> // epoll part
#include <unistd.h> // close socket

//#include <sys/socket.h>
//#include <sys/types.h>
//#include <sys/stat.h>


class http_server{
private:
	const int MAX_EVENTS = 256;

	std::string host;
	std::string port;
	std::string directory;

	int initiate_socket(std::string h, std::string p);
	int set_nonblock(int fd);
	void handle_requests(int fd);
	void proceed_request(int fd);

	std::string build_reply(std::string, std::string);
	
public:
	http_server(std::string, std::string, std::string);
	~http_server();

	void run();
};