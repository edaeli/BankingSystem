#ifndef BANK_H
#define BANK_H

#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <pthread.h>
#include <iostream>

#define SHM_NAME "/shlor"

struct Account {
	int balance = 0;
 	int min_balance = -100;
 	int max_balance = 10000;
 	bool frozen = 0;
};

struct Bank {
	int N;
	pthread_mutex_t mutex;
 	Account accounts[];
};

#endif

