 #include "bank.h"

int main(int argc,char *argv[]){
	if(argc != 2){
		perror("Invalid argument\n");
		return 1;
	}

	int N = std::atoi(argv[1]);

 	int shm_fd = shm_open(SHM_NAME ,O_CREAT | O_RDWR, 0666);

	if (shm_fd == -1) {
    		perror("shm_open");
    		return 1;
	}

 	int size = N * sizeof(Account) + sizeof(Bank);

 	ftruncate(shm_fd, size);

 	auto *bank = (Bank*)mmap(nullptr,size,PROT_READ | PROT_WRITE,MAP_SHARED, shm_fd,0);

	if (bank == MAP_FAILED) {
    		perror("mmap");
    		return 1;
	}

 	bank->N = N;	

	pthread_mutexattr_t attr;
    	pthread_mutexattr_init(&attr);
    	pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);

    	pthread_mutex_init(&bank->mutex, &attr);
	pthread_mutexattr_destroy(&attr);

    	for (int i = 0; i < N; i++) {
        	bank->accounts[i].balance = 0;
        	bank->accounts[i].min_balance = -100;
        	bank->accounts[i].max_balance = 10000;
        	bank->accounts[i].frozen = false;
    	}

    	munmap(bank, size);

    	std::cout << "Bank created\n";

 	close(shm_fd);

}
