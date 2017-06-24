#include "http_server.h"

http_server::http_server(std::string ip, std::string p, std::string dir)
{
	host = ip;
	port = p;
	directory = dir;
}

http_server::~http_server(){}

void http_server::run()
{
	// start socket
	int master_socket = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in SockAddr;
	SockAddr.sin_family = AF_INET;
	SockAddr.sin_port = htons(std::stoi(port)); // port is a private varianble of the class
	inet_aton("63.161.169.137", &SockAddr.sin_addr);

	// bind
	bind(master_socket, (struct sockaddr *)(&SockAddr), sizeof(SockAddr));

	// start to listen
}