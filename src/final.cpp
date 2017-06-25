#include <iostream>     // abort()
#include <unistd.h>     // optarg
#include "http_server.h"


int main(int argc, char *argv[])
{
	std::string ip = "127.0.0.1";
	std::string port = "5432";
	std::string directory = "/tmp/";

	int opt;
	while ((opt = getopt(argc, argv, "h:p:d:")) != -1){
        switch(opt)
        {
            case 'h': 
            	if(optarg) ip = optarg;
            	break;
            case 'p':
            	if(optarg) port = optarg;
            	break;
            case 'd': 
            	if(optarg) directory = optarg;
            	break;
            
            default: 
            	abort();
        }
    }

	http_server h(ip, port, directory);
    h.run();

	return 0;
}
