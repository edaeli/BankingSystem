CXX = g++
CXXFLAGS = -std=c++11 -Wall -pthread
LDFLAGS = -lrt

# Флаги для разных сборок
RELEASE_FLAGS = -O3
DEBUG_FLAGS = -g -O0
COVERAGE_FLAGS = -fprofile-arcs -ftest-coverage

TARGETS = init destroy server client_online

.PHONY: all clean release debug coverage test_valgrind

all: release

# Сборка без отладочной информации
release: CXXFLAGS += $(RELEASE_FLAGS)
release: $(TARGETS)

# Сборка для отладки
debug: CXXFLAGS += $(DEBUG_FLAGS)
debug: $(TARGETS)

# Сборка для измерения покрытия
coverage: CXXFLAGS += $(DEBUG_FLAGS) $(COVERAGE_FLAGS)
coverage: LDFLAGS += --coverage
coverage: clean $(TARGETS)

# Правила для объектных файлов (каждый модуль отдельно)
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Сборка исполняемых файлов
init: init.o shm.o
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

destroy: destroy.o shm.o
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

server: server.o commands.o shm.o
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

client_online: client_online.o
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

# Запуск тестов под Valgrind (memcheck и helgrind)
test_valgrind: debug
	@echo "Running Valgrind memcheck on server..."
	valgrind --tool=memcheck --leak-check=full --error-exitcode=1 ./server & sleep 2 && (echo "balance 0" | ./client_online 127.0.0.1 8080) && (echo "shutdown" | ./client_online 127.0.0.1 8080)
	@echo "Running Valgrind helgrind on server..."
	valgrind --tool=helgrind ./server & sleep 2 && (echo "balance 0" | ./client_online 127.0.0.1 8080) && (echo "shutdown" | ./client_online 127.0.0.1 8080)

clean:
	rm -f *.o $(TARGETS) *.gcno *.gcda *.gcov
