#include "http_server.h"

http_server::http_server(std::string ip, std::string p, std::string dir)
{
	host = ip;
	port = p;
	directory = dir;
}

http_server::~http_server()
{
}

int http_server::initiate_socket(std::string h, std::string p){
	// start socket
	int master_socket = socket(AF_INET, SOCK_STREAM, 0);

	// create sockaddr_in struct
	struct sockaddr_in SockAddr;
	SockAddr.sin_family = AF_INET;
	SockAddr.sin_port = htons(std::stoi(p)); // port is a private varianble of the class
	inet_pton(AF_INET, h.c_str(), &SockAddr.sin_addr);

	// bind
	bind(master_socket, (struct sockaddr *)(&SockAddr), sizeof(SockAddr));

	// make it non block
	set_nonblock(master_socket);

	// start to listen
	listen(master_socket, SOMAXCONN);

	return master_socket;	
}

int http_server::set_nonblock(int fd) {
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

std::string http_server::build_reply(std::string header, std::string data)
{
	std::string reply;

	reply.append(header);
	reply.append("\r\n");
	reply.append("Content-length: ");
	reply.append(std::to_string(data.size()));
	reply.append("\r\n");
	reply.append("Content-Type: text/html");
	reply.append("\r\n\r\n");
	reply.append(data);

	return reply;
}

void http_server::proceed_request(int fd)
{
	// @ Maksim Vlasov comment!!!!
	static char BUFFER[1024];

	// read the request
	int RecvResult = recv(fd, BUFFER, 1024, MSG_NOSIGNAL);
	
	// if we do not like what we read or can not read
	if((RecvResult == 0) && (errno != EAGAIN)){
		shutdown(fd, SHUT_RDWR);
		close(fd);
	}
	// if read was fine and we want to reply
	else if(RecvResult > 0){
		// parse HTTP request: get the file name
		std::string request = BUFFER;
		int start = request.find("/");
		if(start == -1)
			start = 0;
		int end = request.find("?");
		if (end == -1)
			end = request.length();

		std::string path_file;
		if(end>start)
			path_file = request.substr(start, end-start);

		// read from the given file
		std::string data_to_send; 
		FILE* file_in = std::fopen(path_file.c_str(), "r");
		if(file_in){
			std::string data;
			data_to_send = build_reply("HTTP/1.0 404 NOT FOUND", data);
		}
		else{
			std::string data;
			std::fseek(file_in, 0, SEEK_END);
			long fsize = std::ftell(file_in);
			std::fseek(file_in, 0, SEEK_SET); 

			char *string = (char*)std::malloc(fsize + 1);
			std::fread(string, fsize, 1, file_in);

			string[fsize] = 0;
			data = string;
			data_to_send = build_reply("HTTP/1.0 200 OK", data);
		}
		std::fclose(file_in);

		// send the reply
		send(fd, data_to_send.c_str(), data_to_send.length(), MSG_NOSIGNAL);

	}
	return;
}

void http_server::handle_requests(int master_socket)
{
	// create the epoll file distruct
	int epollfd = epoll_create1(0);

	// specify the Epoll
	struct epoll_event ev;
	ev.events = EPOLLIN | EPOLLET;
	ev.data.fd = master_socket;
	
	// connect the pool to the socket
	epoll_ctl(epollfd, EPOLL_CTL_ADD, master_socket, &ev);

	// start to handle requests
	while(1){
		// MAX_EVENTS = 256 (const variable)
		struct epoll_event EVENTS[MAX_EVENTS];

		// check the number of requests
		int N = epoll_wait(epollfd, EVENTS, MAX_EVENTS, -1);

		// loop in requests
		for(int i=0; i<N; i++){

			// if the master socket sends the request we accept it
			if(EVENTS[i].data.fd == master_socket){
				int SlaveSocket = accept(master_socket, 0 ,0);
				set_nonblock(SlaveSocket);
				
				struct epoll_event SlaveEvent;
				SlaveEvent.events = EPOLLIN;
				SlaveEvent.data.fd = SlaveSocket;
				
				epoll_ctl(epollfd, EPOLL_CTL_ADD, SlaveSocket, &SlaveEvent);
			}
			else{
				int recv_fd = EVENTS[i].data.fd;
				proceed_request(recv_fd);
			}
		} // foor loop
	} // while loop

	return;
}

void http_server::run()
{
	// create and start to listen
	int my_socket = initiate_socket(host, port);
	handle_requests(my_socket);
}