#include "bank.h"
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstdio>

Bank *bank_attach(int& shm_fd){
	shm_fd = shm_open(SHM_NAME,O_RDWR, 0666);
	if (shm_fd< 0){
		perror("shm_open");
		return nullptr;
	}

	struct stat st;
 	fstat(shm_fd, &st);
 	void *ptr = mmap(n)
}
