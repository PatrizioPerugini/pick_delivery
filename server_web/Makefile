CC=gcc
CCOPTS=-Wall -std=gnu99 -O3
CXX=g++
CXXOPTS=-Wall -std=gnu++11 -O3

HEADERS=utils.h

.phony=clean all

BINS= main
all:	$(BINS)


%.o:	%.c $(HEADERS)
	$(CC) -c $(CCOPTS) $<

%.o:	%.cpp $(HEADERS)
	$(CXX) -c $(CXXOPTS) $<

main:	main.cpp server_main.o reader.o 
	$(CXX) -o $@ $^ $(CXXOPTS) -lpthread -lwebsockets -lmysqlclient -lssl -lcrypto -L/usr/include/mysql
	


clean:
	rm -f *~ *.o $(BINS)
