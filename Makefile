CXX = g++
CXXFLAGS = -std=c++11 -Wall -pthread
LDFLAGS = -lrt

RELEASE_FLAGS = -O3
DEBUG_FLAGS = -g -O0
COVERAGE_FLAGS = -fprofile-arcs -ftest-coverage

TARGETS = init destroy server client_online

.PHONY: all clean release debug coverage test_valgrind

all: release

release: CXXFLAGS += $(RELEASE_FLAGS)
release: $(TARGETS)

debug: CXXFLAGS += $(DEBUG_FLAGS)
debug: $(TARGETS)

coverage: CXXFLAGS += $(DEBUG_FLAGS) $(COVERAGE_FLAGS)
coverage: LDFLAGS += --coverage
coverage: clean $(TARGETS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

init: init.o shm.o
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

destroy: destroy.o shm.o
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

server: server.o commands.o shm.o
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

client_online: client_online.o
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

test_valgrind: debug
	@echo "Running Valgrind memcheck on server..."
	valgrind --tool=memcheck --leak-check=full --error-exitcode=1 ./server & sleep 2 && (echo "balance 0" | ./client_online 127.0.0.1 8080) && (echo "shutdown" | ./client_online 127.0.0.1 8080)
	@echo "Running Valgrind helgrind on server..."
	valgrind --tool=helgrind ./server & sleep 2 && (echo "balance 0" | ./client_online 127.0.0.1 8080) && (echo "shutdown" | ./client_online 127.0.0.1 8080)

clean:
	rm -f *.o $(TARGETS) *.gcno *.gcda *.gcov
