#include <iostream>
#include <vector>
#include <cstring>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "bank.h"
#include "commands.h"

extern Bank* bank_attach(int& shm_fd);

// Глобальные переменные для синхронизации статистики
int total_requests = 0;
pthread_mutex_t stats_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t stats_cond = PTHREAD_COND_INITIALIZER;
bool server_running = true;
Bank* shared_bank = nullptr;

// Нить для вывода статистики (каждые 5 запросов)
void* stats_thread_routine(void* arg) {
    pthread_mutex_lock(&stats_mutex);
    while (server_running) {
        pthread_cond_wait(&stats_cond, &stats_mutex);
        if (total_requests % 5 == 0 && total_requests > 0) {
            std::cout << "\033[33m[SERVER STATS] Processed " << total_requests << " requests.\033[0m" << std::endl;
        }
    }
    pthread_mutex_unlock(&stats_mutex);
    return nullptr;
}

// Структура для передачи данных в нить клиента
struct ClientInfo {
    int fd;
};

void* client_thread_routine(void* arg) {
	ClientInfo* info = (ClientInfo*)arg;
    	int client_fd = info->fd;
    	delete info;

	char buffer[1024];
    	while (server_running) {
        	memset(buffer, 0, 1024);
        	ssize_t bytes_read = recv(client_fd, buffer, 1024, 0);
        	if (bytes_read <= 0) break;

        	std::string request(buffer);
        	bool shutdown_requested = false;
        
        	// Обработка команды через твой commands.cpp
        	std::string response = process_command(shared_bank, request, shutdown_requested);

        	// Обновляем статистику
        	pthread_mutex_lock(&stats_mutex);
        	total_requests++;
        	pthread_cond_signal(&stats_cond);
        	pthread_mutex_unlock(&stats_mutex);
	
        	send(client_fd, response.c_str(), response.length(), 0);

        	if (shutdown_requested) {
            		server_running = false;
            		std::cout << "Shutdown command received. Closing server...\n";
            		exit(0); // В учебных целях завершаем весь процесс
        	}
    	}
    	close(client_fd);
    	return nullptr;
}

int main() {
    	int shm_fd;
    	shared_bank = bank_attach(shm_fd);
    	if (!shared_bank) return 1;

    	int server_sock = socket(AF_INET, SOCK_STREAM, 0);
    	int opt = 1;
    	setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    	sockaddr_in addr;
    	addr.sin_family = AF_INET;
    	addr.sin_port = htons(8080);
    	addr.sin_addr.s_addr = INADDR_ANY;

    	if (bind(server_sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        	perror("Bind failed");
        	return 1;
    	}

    	listen(server_sock, 10);
    
    	pthread_t stats_tid;
    	pthread_create(&stats_tid, nullptr, stats_thread_routine, nullptr);

    	std::cout << "Bank Server Online on port 8080. Waiting for clients...\n";

    	while (server_running) {
        	int client_fd = accept(server_sock, nullptr, nullptr);
        	if (client_fd < 0) break;

        	pthread_t client_tid;
        	ClientInfo* info = new ClientInfo{client_fd};
        	pthread_create(&client_tid, nullptr, client_thread_routine, info);
        	pthread_detach(client_tid);
    	}

    	close(server_sock);
    	return 0;
}
