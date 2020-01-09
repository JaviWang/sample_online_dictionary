BIN=server
OBJ=main.o server.o t_net.o
CC=gcc 

$(BIN):$(OBJ)
	@$(CC) -lsqlite3 -o $(BIN) $(OBJ) 
	@echo "Compile successfully"
%.o:%.c
	@$(CC) -c -o $@ $<

clean:
	@rm $(BIN) $(OBJ)
	@echo "Cleanup Successfully"

