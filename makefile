CC = g++
CFLAGS = -Wall -std=c++1z
DEPS = globalheader.h
OBJ = globalheader.o get_fn.o client.o read_input.o share_fn.o sha1.o \
		remove_fn.o client_init.o server_init.o peer_server.o
OBJ_T = globalheader.o tracker.o server_init.o

%.o: %.cpp $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $< -lcrypto -pthread

all: client tracker 

client: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ -lcrypto -pthread
	
tracker: $(OBJ_T)
	$(CC) $(CFLAGS) -o $@ $^ -lcrypto -pthread

clean:
	rm -f *.o client tracker
