#pragma once

#include <iostream> 		// cout
#include <string> 			// std::string, stoi
#include <fcntl.h> 			// flag FIOBIO in set_nonblock
#include <arpa/inet.h> 		// inet_pton()
#include <sys/epoll.h> 		// epoll part
#include <unistd.h> 		// close socket

#include "handler.cpp"

#define MAX_EVENTS 32

class http_server
{
public:
	http_server(std::string, std::string, std::string);
	~http_server();
	void run();
private:
	std::string host;
	std::string port;
	std::string directory;
	handler* my_handler;

	int set_nonblock(int);
	int initiate_socket(std::string, std::string);
	void handle_requests(int);
};

http_server::http_server(std::string h, std::string p, std::string d){
	host = h;
	port = p;
	directory = d;
	my_handler = new handler();
}
http_server::~http_server(){
	delete my_handler;
}

int http_server::set_nonblock(int fd) {
	int flags;
#if defined(O_NONBLOCK)
	if(-1 == (flags = fcntl(fd, F_GETFL, 0))){
		flags = 0;
	}
	return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
#else
	flags = 1;
	return ioctl(fd, FIOBIO, &flags);
#endif
}

int http_server::initiate_socket(std::string h, std::string p){
	int master_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	struct sockaddr_in SockAddr;
	SockAddr.sin_family = AF_INET;
	SockAddr.sin_port = htons(std::stoi(p));
	SockAddr.sin_addr.s_addr = inet_addr(h.c_str());

	bind(master_socket, (struct sockaddr *)(&SockAddr), sizeof(SockAddr));
	set_nonblock(master_socket);
	listen(master_socket, SOMAXCONN);

	return master_socket;
}

void http_server::handle_requests(int master_socket){
	int Epoll = epoll_create1(0);

	struct epoll_event Event;
	Event.events = EPOLLIN;
	Event.data.fd = master_socket;
	
	epoll_ctl(Epoll, EPOLL_CTL_ADD, master_socket, &Event);

	while(1){
		struct epoll_event Events[MAX_EVENTS];
		int N = epoll_wait(Epoll, Events, MAX_EVENTS, -1);

		for(int i=0; i<N; i++){
			if(Events[i].data.fd == master_socket){
				int SlaveSocket = accept(master_socket, 0 ,0);
				set_nonblock(SlaveSocket);
				
				struct epoll_event SlaveEvent;
				SlaveEvent.events = EPOLLIN;
				SlaveEvent.data.fd = SlaveSocket;
				
				epoll_ctl(Epoll, EPOLL_CTL_ADD, SlaveSocket, &SlaveEvent);
			}
			else{
				my_handler->reply(Events[i].data.fd, directory);
			}
		} // foor loop
	} // while loop
	return;
}

void http_server::run(){
	int MasterSocket = initiate_socket(host,port);
	handle_requests(MasterSocket);
	return;
}
// EOF