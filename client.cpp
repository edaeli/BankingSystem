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
			std::cin >> A >> B;

			if(A < 0 || A >= bank->N || B < 0 || B >= bank->N){
				std::cout << "Invalid account\n";
				continue;
			}

			if (!(std::cin >> X)){
    				std::cout << "Invalid amount\n";
    				std::cin.clear();               
    				std::cin.ignore(1000, '\n'); 
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

			if (!(std::cin >> X)){
                                std::cout << "Invalid amount\n";
                                std::cin.clear();
                                std::cin.ignore(1000, '\n');
                                continue;
                        }


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

			if (!(std::cin >> X)){
                                std::cout << "Invalid amount\n";
                                std::cin.clear();
                                std::cin.ignore(1000, '\n');
                                continue;
                        }


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

                        std::cout << "Subbed " << X << " to all accounts\n";

                        pthread_mutex_unlock(&bank->mutex);
                }

		else if (cmd == "setmin"){

    			int A, X;
    			std::cin >> A;

    			if (A < 0 || A >= bank->N){
        			std::cout << "Invalid account\n";
        			continue;
    			}

			if (!(std::cin >> X)){
                                std::cout << "Invalid amount\n";
                                std::cin.clear();
                                std::cin.ignore(1000, '\n');
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
                        std::cin >> A;


                        if (A < 0 || A >= bank->N){
                                std::cout << "Invalid account\n";
                                continue;
                        }
			
			if (!(std::cin >> X)){
                                std::cout << "Invalid amount\n";
                                std::cin.clear();
                                std::cin.ignore(1000, '\n');
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

		
		else if (cmd == "help"){
            		std::cout << "Available commands:\n"
                      << "balance A      - show current balance of account A\n"
                      << "min A          - show min allowed balance of account A\n"
                      << "max A          - show max allowed balance of account A\n"
                      << "freeze A       - freeze account A\n"
                      << "unfreeze A     - unfreeze account A\n"
                      << "transfer A B X - transfer X units from A to B\n"
                      << "addall X       - add X units to all accounts\n"
                      << "suball X       - subtract X units from all accounts\n"
                      << "setmin A X     - set min balance of account A to X\n"
                      << "setmax A X     - set max balance of account A to X\n"
                      << "exit           - exit client\n"
                      << "help           - show this message\n";
        	}

        	else if (cmd == "exit"){
            		std::cout << "Exiting client.\n";
            		break;
        	}

		else {
			std::cout << "Invalid command\n";
			continue;	
		}

	}

	return 0;
}
