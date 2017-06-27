#include <string>
#include <stdlib.h>     // abort()
#include <unistd.h>
#include "http_server.cpp"

int main(int argc, char *argv[])
{
	std::string host;
	std::string port;
	std::string dir;

	int opt;
	while ((opt = getopt(argc, argv, "h:p:d:")) != -1){
        switch(opt)
        {
            case 'h': 
            	if(optarg) host = optarg;
            	break;
            case 'p':
            	if(optarg) port = optarg;
            	break;
            case 'd': 
            	if(optarg) dir = optarg;
            	break;
            
            default: 
            	abort();
        }
    }

	daemon(0,0);

	http_server s(host, port, dir);
	s.run();

	return 0;
}