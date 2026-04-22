# Transparent Bank Online (Multithreaded IPC Project)

This project is a high-performance banking system implemented in C++. It utilizes **Shared Memory** for data persistence and **TCP/IP Sockets** for remote management. The system is designed to be thread-safe and supports multiple concurrent clients.

## 🚀 Features

*   **Inter-Process Communication (IPC):** Uses POSIX Shared Memory to store bank data.
*   **Multithreaded Server:** Handles each client in a separate thread.
*   **Condition Variables:** A dedicated statistics thread monitors system activity and reports every 5 requests using `pthread_cond_wait`.
*   **Thread Safety:** Account integrity is protected using `pthread_mutex`.
*   **Remote Client:** A terminal-based client with intelligent colored output using Regex.
*   **Robust Logic:** Checks for frozen accounts, minimum/maximum balance limits, and invalid inputs.

---

## 🛠 Compilation

The project uses a `Makefile` with specific targets for different needs:

*   `make` or `make release`: Builds optimized binaries for production.
*   `make debug`: Builds binaries with debug symbols (`-g`) for Valgrind/GDB.
*   `make coverage`: Builds with instrumentation to measure test coverage.
*   `make clean`: Removes all temporary files and binaries.

---

## 📖 How to Run

### 1. Initialize the Bank
First, create the shared memory segment. Replace `<N>` with the number of accounts:
```bash
./init 5
```

### 2. Start the Server
Launch the multithreaded server (it listens on port 8080 by default):
```bash
./server
```

### 3. Connect the Client
In a new terminal, connect to the server:
```bash
./client_online 127.0.0.1 8080
```

### 4. Cleanup
After shutting down the server, remove the shared memory segment:
```bash
./destroy
```

---

## 🧪 Testing & Verification

### Automated Test Script
We have included a bash script that automates the entire flow:
```bash
chmod +x run_tests.sh
./run_tests.sh
```

### Memory & Thread Safety (Valgrind)
To verify that the project is free of memory leaks and data races:
```bash
make debug
# Check for leaks
valgrind --leak-check=full ./server
# Check for thread synchronization issues
valgrind --tool=helgrind ./server
```

---

## ⌨️ Available Commands

| Command | Description |
| :--- | :--- |
| `balance A` | Show current balance of account A |
| `min A` | Show minimum allowed balance for account A |
| `max A` | Show maximum allowed balance for account A |
| `freeze A` | Disable transactions for account A |
| `unfreeze A` | Enable transactions for account A |
| `transfer A B X` | Transfer X amount from account A to B |
| `addall X` | Add X units to all non-frozen accounts |
| `suball X` | Subtract X units from all non-frozen accounts |
| `setmin A X` | Change minimum limit for account A |
| `setmax A X` | Change maximum limit for account A |
| `shutdown` | Gracefully shut down the server |
| `exit` | Close the client connection |

---

## 📁 Project Structure

*   `bank.h`: Shared structures and SHM definitions.
*   `shm.cpp`: Shared memory attachment logic.
*   `commands.cpp`: The "Engine" — processes all banking business logic.
*   `server.cpp`: Socket handling, threading, and statistics.
*   `client_online.cpp`: User interface and networking.
*   `colorprint.h`: Regex-based utility for coloring output.
