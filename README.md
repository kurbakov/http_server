# HTTP server

Tech stack:
- Cmake
- Epoll

The server should be started with the following command:
```bash
./final -h <ip> -p <port> -d <directory>
```
The server consists of the following files:
- main.cpp -> File to start the server
- http_server.cpp -> Epool (setup the server and accept signals)
- handler.cpp -> Implementation of GET, PUT, etc.

Currently I implemented only GET command. Feel free to add any other method.

This code is the final exame for the STEPIK couurse 149: multithreading C/C++.
The last test was done 04.09.17 The score: 300/300.

Possible areas for improvements:
1. Add multithreading
2. Add PUT
