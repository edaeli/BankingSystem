#include "commands.h"
#include <sstream>

std::string process_command(Bank* bank, const std::string& input, bool& shutdown_flag) {
    std::istringstream iss(input);
    std::string cmd;
    iss >> cmd;


    if (cmd == "shutdown") {
        shutdown_flag = true;
        return "Server shutting down...\n";
    }

    if (cmd == "balance") {
	int A;
        if (!(iss >> A))
                return "Invalid input\n";

        if (A < 0 || A >= bank->N)
            	return "Invalid account\n";

        pthread_mutex_lock(&bank->mutex);
        int bal = bank->accounts[A].balance;
        pthread_mutex_unlock(&bank->mutex);

        return "Balance: " + std::to_string(bal) + "\n";
    }

   else if (cmd == "min"){

    	int A;
	if (!(iss >> A))
    		return "Invalid input\n";

    	if (A < 0 || A >= bank->N)
        	return "Invalid account\n";

    	pthread_mutex_lock(&bank->mutex);

    	int bal = bank->accounts[A].min_balance;

    	pthread_mutex_unlock(&bank->mutex);

	return "Min balance: " + std::to_string(bal) + "\n";
   }

   else if (cmd == "max"){

	int A;
        if (!(iss >> A))
                return "Invalid input\n";

        if (A < 0 || A >= bank->N)
                return "Invalid account\n";

        pthread_mutex_lock(&bank->mutex);

        int bal = bank->accounts[A].max_balance;

        pthread_mutex_unlock(&bank->mutex);

        return "Max balance: " + std::to_string(bal) + "\n";
   }

   else if (cmd == "freeze") {
    	int A;

    	if (!(iss >> A))
        	return "Invalid input\n";

    	if (A < 0 || A >= bank->N)
        	return "Invalid account\n";

    	pthread_mutex_lock(&bank->mutex);

    	bank->accounts[A].frozen = true;

    	pthread_mutex_unlock(&bank->mutex);

  	return "Account " + std::to_string(A) + " frozen\n";
  }

  else if (cmd == "unfreeze") {
        int A;

        if (!(iss >> A))
                return "Invalid input\n";

        if (A < 0 || A >= bank->N)
                return "Invalid account\n";

        pthread_mutex_lock(&bank->mutex);

        bank->accounts[A].frozen = false;

        pthread_mutex_unlock(&bank->mutex);

        return "Account " + std::to_string(A) + " unfrozen\n";
  }
  else if (cmd == "transfer") {
  	int A, B, X;

    	if (!(iss >> A >> B))
        	return "Invalid input\n";

    	if (A < 0 || A >= bank->N || B < 0 || B >= bank->N)
        	return "Invalid account\n";

    	if (!(iss >> X))
        	return "Invalid amount\n";

    	if (X <= 0)
        	return "Invalid amount\n";

    	pthread_mutex_lock(&bank->mutex);

    	if (bank->accounts[A].frozen || bank->accounts[B].frozen) {
        	pthread_mutex_unlock(&bank->mutex);
        	return "Account frozen\n";
    	}

    	if (bank->accounts[A].balance - X < bank->accounts[A].min_balance) {
        	pthread_mutex_unlock(&bank->mutex);
        	return "Transfer would violate min balance\n";
    	}

    	if (bank->accounts[B].balance + X > bank->accounts[B].max_balance) {
        	pthread_mutex_unlock(&bank->mutex);
        	return "Transfer would violate max balance\n";
    	}

    	bank->accounts[A].balance -= X;
    	bank->accounts[B].balance += X;

    	pthread_mutex_unlock(&bank->mutex);

    	return "Transferred " + std::to_string(X) +
           " from " + std::to_string(A) +
           " to " + std::to_string(B) + "\n";
  }
 
  else if (cmd == "addall") {
  	int X;

  	if (!(iss >> X))
        	return "Invalid amount\n";

	if (X <= 0)
    		return "Invalid amount\n";


    	pthread_mutex_lock(&bank->mutex);

    	std::string result = "";

    	for (int i = 0; i < bank->N; i++) {

        	if (bank->accounts[i].frozen) {
            		result += "Account " + std::to_string(i) + " is frozen\n";
            		continue;
        	}

        	if (bank->accounts[i].balance + X > bank->accounts[i].max_balance) {
            		result += "Account " + std::to_string(i) + " exceeds max balance\n";
            		continue;
        	}

        	bank->accounts[i].balance += X;
    	}	

    	pthread_mutex_unlock(&bank->mutex);

    	result += "Added " + std::to_string(X) + " to all accounts\n";

    	return result;
  }

  else if (cmd == "suball") {
        int X;

        if (!(iss >> X))
                return "Invalid amount\n";

	if (X <= 0)
   		 return "Invalid amount\n";


        pthread_mutex_lock(&bank->mutex);

        std::string result = "";

        for (int i = 0; i < bank->N; i++) {

                if (bank->accounts[i].frozen) {
                        result += "Account " + std::to_string(i) + " is frozen\n";
                        continue;
                }

                if (bank->accounts[i].balance - X < bank->accounts[i].min_balance) {
                        result += "Account " + std::to_string(i) + " violates min balance\n";
                        continue;
                }

                bank->accounts[i].balance -= X;
        }

        pthread_mutex_unlock(&bank->mutex);

        result += "Subbed " + std::to_string(X) + " to all accounts\n";

        return result;
  }

 
  else if (cmd == "setmin") {
  	int A, X;

    	if (!(iss >> A))
        	return "Invalid input\n";

    	if (A < 0 || A >= bank->N)
        	return "Invalid account\n";

    	if (!(iss >> X))
        	return "Invalid amount\n";

    	pthread_mutex_lock(&bank->mutex);

    	if (X > bank->accounts[A].balance) {
        	pthread_mutex_unlock(&bank->mutex);
        	return "New min greater than current balance\n";
    	}

    	bank->accounts[A].min_balance = X;

    	pthread_mutex_unlock(&bank->mutex);

    	return "Min balance of account " + std::to_string(A) +
           " set to " + std::to_string(X) + "\n";
  }
 
  else if (cmd == "setmax") {
        int A, X;

        if (!(iss >> A))
                return "Invalid input\n";

        if (A < 0 || A >= bank->N)
                return "Invalid account\n";

        if (!(iss >> X))
                return "Invalid amount\n";

        pthread_mutex_lock(&bank->mutex);

        if (X < bank->accounts[A].balance) {
                pthread_mutex_unlock(&bank->mutex);
                return "New max less than current balance\n";
        }

        bank->accounts[A].max_balance = X;

        pthread_mutex_unlock(&bank->mutex);

        return "Max balance of account " + std::to_string(A) +
           " set to " + std::to_string(X) + "\n";
  }

  else if (cmd == "help") {
  	return
        	"Available commands:\n"
        	"balance A      - show current balance of account A\n"
        	"min A          - show min allowed balance of account A\n"
        	"max A          - show max allowed balance of account A\n"
        	"freeze A       - freeze account A\n"
        	"unfreeze A     - unfreeze account A\n"
        	"transfer A B X - transfer X units from A to B\n"
        	"addall X       - add X units to all accounts\n"
        	"suball X       - subtract X units from all accounts\n"
        	"setmin A X     - set min balance of account A to X\n"
        	"setmax A X     - set max balance of account A to X\n"
        	"exit           - exit client\n"
        	"help           - show this message\n";
  }

  else if (cmd == "exit") {
  	return "exit\n";
  }

  else
    	return "Invalid command\n";
  
}

