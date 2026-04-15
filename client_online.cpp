#include <iostream>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include "colorprint.h"

int main(int argc, char* argv[]) {
    	if (argc < 3) {
        	std::cout << "Usage: ./client <IP> <PORT>\n";
        	return 1;
    	}

    	int sock = socket(AF_INET, SOCK_STREAM, 0);
    	sockaddr_in serv_addr;
    	serv_addr.sin_family = AF_INET;
    	serv_addr.sin_port = htons(std::stoi(argv[2]));
    	inet_pton(AF_INET, argv[1], &serv_addr.sin_addr);

    	if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        	perror("Connection failed");
        	return 1;
    	}

    	std::vector<std::string> green = {"Success", "Transferred","Added" ,"unfrozen", "Balance"};
    	std::vector<std::string> red = {"Invalid", "Error", "frozen", "violate", "exceeds"};
    	Painter painter(std::cout, green, red);

    	std::string line;
    	char buffer[4096];

    	std::cout << "Connected to Bank. Type commands (or 'exit' to quit):\n";
    
    	while (true) {
        	std::cout << "> ";
        	if (!std::getline(std::cin, line) || line == "exit") break;

        	send(sock, line.c_str(), line.length(), 0);
        
        	memset(buffer, 0, 4096);
        	ssize_t bytes = recv(sock, buffer, 4096, 0);
        	if (bytes <= 0) break;

        	painter.print(std::string(buffer));
        
        	if (line == "shutdown") break;
    	}

    	close(sock);
    	return 0;
}
