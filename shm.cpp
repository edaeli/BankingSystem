#include "bank.h"

Bank *bank_attach(int& shm_fd){

	shm_fd = shm_open(SHM_NAME,O_RDWR, 0666);

	if (shm_fd < 0){
		perror("shm_open");
		return nullptr;
	}

	struct stat st;
	fstat(shm_fd, &st);

	void *ptr = mmap(nullptr,
	                 st.st_size,
	                 PROT_READ | PROT_WRITE,
	                 MAP_SHARED,
	                 shm_fd,
	                 0);

	if (ptr == MAP_FAILED){
		perror("mmap");
		return nullptr;
	}

	return (Bank*)ptr;
}

