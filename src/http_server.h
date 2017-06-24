#include <arpa/inet.h>
#include <fcntl.h>

#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string>

class http_server{
private:
	std::string host;
	std::string port;
	std::string directory;
	
public:
	http_server(std::string, std::string, std::string);
	~http_server();

	void run();
};