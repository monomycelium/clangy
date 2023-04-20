CC = zig cc
CFLAGS = -O3 -pipe -march=native
LIBRARY_SRC = network.c
LIBRARY_OBJ = $(LIBRARY_SRC:.c=.o)
CLIENT_SRC = hacker_client.c
SERVER_SRC = hacker_server.c
CLIENT_OBJ = $(CLIENT_SRC:.c=.o)
SERVER_OBJ = $(SERVER_SRC:.c=.o)
CLIENT_EXEC = hacker_client
SERVER_EXEC = hacker_server

all: $(CLIENT_EXEC) $(SERVER_EXEC)

client $(CLIENT_EXEC): $(CLIENT_OBJ) $(LIBRARY_OBJ)
	$(CC) $(CFLAGS) `pkg-config --libs --static sqlite3` $(CLIENT_OBJ) $(LIBRARY_OBJ) -o $(CLIENT_EXEC)

server $(SERVER_EXEC): $(SERVER_OBJ) $(LIBRARY_OBJ)
	$(CC) $(CFLAGS) $(SERVER_OBJ) $(LIBRARY_OBJ) -o $(SERVER_EXEC)

$(CLIENT_OBJ): $(CLIENT_SRC)
	$(CC) -c $(CFLAGS) $(CLIENT_SRC) -o $(CLIENT_OBJ)

$(SERVER_OBJ): $(SERVER_SRC)
	$(CC) -c $(CFLAGS) $(SERVER_SRC) -o $(SERVER_OBJ)

$(LIBRARY_OBJ): $(LIBRARY_SRC)
	$(CC) -c $(CFLAGS) $(LIBRARY_SRC) -o $(LIBRARY_OBJ)

clean:
	rm -fv $(CLIENT_EXEC) $(SERVER_EXEC) $(CLIENT_OBJ) $(SERVER_OBJ) $(LIBRARY_OBJ) hacker.csv
