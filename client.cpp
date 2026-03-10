#include "bank.h"
#include <string>

extern Bank* bank_attach(int& shm_fd);

int main(){

	int shm_fd;

	Bank* bank = bank_attach(shm_fd);

	if (bank == nullptr){
		std::cerr << "Cannot attach bank\n";
		return 1;
	}

	std::string cmd;


	while (std::cin >> cmd){

		if (cmd == "balance"){

			int A;
			std::cin >> A;

			if (A < 0 || A >= bank->N){
				std::cout << "Invalid account\n";
				continue;
			}

			pthread_mutex_lock(&bank->mutex);

			std::cout << "Balance: "
			          << bank->accounts[A].balance
			          << "\n";

			pthread_mutex_unlock(&bank->mutex);
		}
		else if (cmd == "min"){

    			int A;
    			std::cin >> A;

    			if (A < 0 || A >= bank->N){
        			std::cout << "Invalid account\n";
        			continue;
    			}

    			pthread_mutex_lock(&bank->mutex);

    			std::cout << "Min balance: "
              			  << bank->accounts[A].min_balance
              			  << "\n";

    			pthread_mutex_unlock(&bank->mutex);
		}

		else if (cmd == "max"){
		
			int A;
    			std::cin >> A;

    			if (A < 0 || A >= bank->N){
        			std::cout << "Invalid account\n";
        			continue;
    			}

    			pthread_mutex_lock(&bank->mutex);

    			std::cout << "Max balance: "
              			  << bank->accounts[A].max_balance
              			  << "\n";

    			pthread_mutex_unlock(&bank->mutex);
		}

		else if (cmd  == "freeze"){
			
			int A;
			std::cin >> A;
			
			if(A < 0 || A >= bank->N){
				std::cout << "Invalid account\n";
				continue;
			}

			pthread_mutex_lock(&bank->mutex);

			bank->accounts[A].frozen = true;

			std::cout << "Account " << A << "frozen\n";

			pthread_mutex_unlock(&bank->mutex);	

		}

		else if (cmd  == "unfreeze"){

                        int A;
                        std::cin >> A;

                        if(A < 0 || A >= bank->N){
                                std::cout << "Invalid account\n";
                                continue;
                        }

                        pthread_mutex_lock(&bank->mutex);

                        bank->accounts[A].frozen = false;

                        std::cout << "Account " << A << " unfrozen\n";

                        pthread_mutex_unlock(&bank->mutex);

                }
		
		else if (cmd == "transfer"){
			int A, B, X;
			std::cin >> A >> B >> X;

			if(A < 0 || A >= bank->N || B < 0 || B >= bank->N){
				std::cout << "Invalid account\n";
				continue;
			}

			if(X < 0){
				std::cout << "Invalid amount\n";
				continue;
			}

			pthread_mutex_lock(&bank->mutex);

			if(bank->accounts[A].frozen || bank->accounts[B].frozen){
				std::cout << "Account frozen\n";
				pthread_mutex_unlock(&bank->mutex);
				continue;
			}

			if(bank->accounts[A].balance - X < bank->accounts[A].min_balance){
				std::cout << "Transfer would violate min balance\n";
				pthread_mutex_unlock(&bank->mutex);
				continue;
			}

			if(bank->accounts[B].balance + X > bank->accounts[B].max_balance){
                                std::cout << "Transfer would violate max balance\n";
                                pthread_mutex_unlock(&bank->mutex);
                                continue;
                        }

			bank->accounts[A].balance -= X;
			bank->accounts[B].balance += X;
			


			std::cout << "Transfered " << X << " from " << A << " to " << B << std::endl;

			pthread_mutex_unlock(&bank->mutex);	


		}

		else if (cmd == "addall"){

    			int X;
    			std::cin >> X;

   			pthread_mutex_lock(&bank->mutex);

    			for (int i = 0; i < bank->N; i++){

        			if (bank->accounts[i].frozen){
            				std::cout << "Account " << i << " is frozen\n";
            				continue;
        			}

        			if (bank->accounts[i].balance + X > bank->accounts[i].max_balance){
            				std::cout << "Account " << i << " exceeds max balance\n";
            				continue;
        			}

        			bank->accounts[i].balance += X;
    			}

    			std::cout << "Added " << X << " to all accounts\n";

    			pthread_mutex_unlock(&bank->mutex);
		}

		else if (cmd == "suball"){

                        int X;
                        std::cin >> X;

                        pthread_mutex_lock(&bank->mutex);

                        for (int i = 0; i < bank->N; i++){

                                if (bank->accounts[i].frozen){
                                        std::cout << "Account " << i << " is frozen\n";
                                        continue;
                                }

                                if (bank->accounts[i].balance - X < bank->accounts[i].min_balance){
                                        std::cout << "Account " << i << " violates min balance\n";
                                        continue;
                                }

                                bank->accounts[i].balance -= X;
                        }

                        std::cout << "Subed " << X << " to all accounts\n";

                        pthread_mutex_unlock(&bank->mutex);
                }

		else if (cmd == "setmin"){

    			int A, X;
    			std::cin >> A >> X;

    			if (A < 0 || A >= bank->N){
        			std::cout << "Invalid account\n";
        			continue;
    			}

    			pthread_mutex_lock(&bank->mutex);

    			if (X > bank->accounts[A].balance){
        			std::cout << "New min greater than current balance\n";
        			pthread_mutex_unlock(&bank->mutex);
        			continue;
    			}

    			bank->accounts[A].min_balance = X;

    			std::cout << "Min balance of account " << A << " set to " << X << "\n";

    			pthread_mutex_unlock(&bank->mutex);

		}

		else if (cmd == "setmax"){

                        int A, X;
                        std::cin >> A >> X;

                        if (A < 0 || A >= bank->N){
                                std::cout << "Invalid account\n";
                                continue;
                        }

                        pthread_mutex_lock(&bank->mutex);

                        if (X < bank->accounts[A].balance){
                                std::cout << "New max less than current balance\n";
                                pthread_mutex_unlock(&bank->mutex);
                                continue;
                        }

                        bank->accounts[A].max_balance = X;

                        std::cout << "Max balance of account " << A << " set to " << X << "\n";

                        pthread_mutex_unlock(&bank->mutex);

                }


		else {
			std::cout << "Invalid command\n";
			continue;	
		}

	}

	return 0;
}
