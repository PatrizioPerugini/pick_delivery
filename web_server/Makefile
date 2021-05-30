CC=gcc
CCOPTS=-Wall -std=gnu99 -O3
CXX=g++
CXXOPTS=-Wall -std=gnu++11 -O3

.phony=clean all

BINS= server_main
all:	$(BINS)


%.o:	%.c $(HEADERS)
	$(CC) -c $(CCOPTS) $<

%.o:	%.cpp $(HEADERS)
	$(CXX) -c $(CXXOPTS) $<

server_main:	server_main.cpp 
	$(CXX) -o $@ $^ $(CXXOPTS) -lpthread -lwebsockets


clean:
	rm -f *~ *.o $(BINS)
