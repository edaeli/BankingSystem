#include "bank.h"

int main() {

    if (shm_unlink(SHM_NAME) == -1) {
        perror("shm_unlink");
        return 1;
    }

    std::cout << "Bank destroyed\n";

    return 0;
}

