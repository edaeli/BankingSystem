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

struct Account {
	int balance = 0 ;
 	int min_balance = 0;
 	int max_balance = 2147364847;
 	bool frozen = 0;
};

struct Bank {
	int N;
 	Account accounts[];
};

#endif
