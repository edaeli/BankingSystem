CXX = g++
CXXFLAGS = -Wall -Wextra -pthread

all: init destroy client

init: init.o
	$(CXX) init.o -o init $(CXXFLAGS)

destroy: destroy.o
	$(CXX) destroy.o -o destroy

client: client.o shm.o
	$(CXX) client.o shm.o -o client $(CXXFLAGS)

init.o: init.cpp bank.h
	$(CXX) $(CXXFLAGS) -c init.cpp

destroy.o: destroy.cpp bank.h
	$(CXX) $(CXXFLAGS) -c destroy.cpp

client.o: client.cpp bank.h
	$(CXX) $(CXXFLAGS) -c client.cpp

shm.o: shm.cpp bank.h
	$(CXX) $(CXXFLAGS) -c shm.cpp

clean:
	rm -f *.o init destroy client

