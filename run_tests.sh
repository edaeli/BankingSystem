#!/bin/bash

echo "=== Running Automatic Banking System Tests ==="

# 1. Cleanup old data and processes
./destroy 2>/dev/null
killall server 2>/dev/null

# 2. Initialize the bank with 5 accounts
echo "[1/5] Initializing bank with 5 accounts..."
./init 5

# 3. Start the server in the background
echo "[2/5] Starting server in background mode..."
./server > server_output.log 2>&1 &
SERVER_PID=$!

# Give the server a moment to start
sleep 1

# 4. Run the client with a set of test commands
echo "[3/5] Sending commands via client..."

# Using a Heredoc to automate the client input
./client_online 127.0.0.1 8080 << 'EOF'
addall 100
balance 0
balance 1
transfer 0 1 50
freeze 2
transfer 2 1 10
unfreeze 2
setmin 0 -500
setmax 0 2000
balance 1
shutdown
EOF

echo "[4/5] Commands processed. Checking server logs..."

sleep 1

echo "------------------------------------------------"
echo "--- Server Log Output (Check for statistics) ---"
echo "------------------------------------------------"
cat server_output.log
echo "------------------------------------------------"

echo "[5/5] Destroying bank and cleaning up memory..."
./destroy

echo "=== All Tests Completed Successfully ==="
