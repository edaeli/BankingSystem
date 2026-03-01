 #include "bank.h"

int main(int argc,char *argv[]){
	if(argc != 2){
		perror("Invalid argument\n");
		return 1;
	}

	int N = std::atoi(argv[1]);

 	int shm_fd = shm_open("/shlor",O_CREAT | O_RDWR, 0666);
 	int size = N * sizeof(Account) + sizeof(Bank);

 	ftruncate(shm_fd, size);

 	auto *bank = (Bank*)mmap(nullptr,size,PROT_READ | PROT_WRITE,MAP_SHARED, shm_fd,0);

 	bank->N = N;
 
 	for(int i = 0;i < N;i++){
  		std::cout << bank->accounts[i].balance; 
 	}

 	close(shm_fd);

}
