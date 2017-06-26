#include <iostream> 		// cout
#include <string> 			// std::string, stoi
#include <fcntl.h> 			// flag FIOBIO in set_nonblock
#include <arpa/inet.h> 		// inet_pton()
#include <sys/epoll.h> 		// epoll part
#include <unistd.h> 		// close socket

#define MAX_EVENTS 32


class server{
public:
	server(std::string, std::string, std::string);
	~server();
	void run();
private:
	std::string host;
	std::string port;
	std::string directory;

	int set_nonblock(int);
	int initiate_socket(std::string, std::string);
	void handle_requests(int);
};

server::server(std::string h, std::string p, std::string d){
	host = h;
	port = p;
	directory = d;
}

server::~server(){}


int server::set_nonblock(int fd) {
        int flags;
#if defined(O_NONBLOCK)
        if(-1 == (flags = fcntl(fd, F_GETFL, 0)))
                flags = 0;
        return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
#else
        flags = 1;
        return ioctl(fd, FIOBIO, &flags);
#endif
}

int server::initiate_socket(std::string h, std::string p){
	// start socket
	int master_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	// create sockaddr_in struct
	struct sockaddr_in SockAddr;
	SockAddr.sin_family = AF_INET;
	SockAddr.sin_port = htons(std::stoi(p));
	SockAddr.sin_addr.s_addr = inet_addr(h.c_str());

	// bind
	bind(master_socket, (struct sockaddr *)(&SockAddr), sizeof(SockAddr));

	// make it non block
	set_nonblock(master_socket);

	// start to listen
	listen(master_socket, SOMAXCONN);

	return master_socket;	
}

void server::handle_requests(int master_socket)
{
	// create the epoll file distruct
	int Epoll = epoll_create1(0);

	// specify the Epoll
	struct epoll_event Event;
	Event.events = EPOLLIN;
	Event.data.fd = master_socket;
	
	// connect the pool to the socket
	epoll_ctl(Epoll, EPOLL_CTL_ADD, master_socket, &Event);

	// start to handle requests
	while(1){
		// MAX_EVENTS = 256 (const variable)
		struct epoll_event Events[MAX_EVENTS];

		// check the number of requests
		int N = epoll_wait(Epoll, Events, MAX_EVENTS, -1);

		// loop in requests
		for(int i=0; i<N; i++){

			// if the master socket sends the request we accept it
			if(Events[i].data.fd == master_socket){
				int SlaveSocket = accept(master_socket, 0 ,0);
				set_nonblock(SlaveSocket);
				
				struct epoll_event SlaveEvent;
				SlaveEvent.events = EPOLLIN;
				SlaveEvent.data.fd = SlaveSocket;
				
				epoll_ctl(Epoll, EPOLL_CTL_ADD, SlaveSocket, &SlaveEvent);
			}
			else{
				static char BUFFER[1024];

				// read the request
				int RecvResult = recv(Events[i].data.fd, BUFFER, 1024, MSG_NOSIGNAL);
				
				// if we do not like what we read or can not read
				if((RecvResult == 0) && (errno != EAGAIN)){
					shutdown(Events[i].data.fd, SHUT_RDWR);
					close(Events[i].data.fd);
				}
				else if(RecvResult > 0){
					send(Events[i].data.fd, BUFFER, RecvResult, MSG_NOSIGNAL);
				}
			}
		} // foor loop
	} // while loop

	return;
}

void server::run(){
	int MasterSocket = initiate_socket(host,port);
	handle_requests(MasterSocket);
}


int main(int argc, char const *argv[])
{
	std::string host = "127.0.0.1";
	std::string port = "12345";
	std::string dr = "/tmp";

	server s(host,port,dr);
	s.run();

	return 0;
}