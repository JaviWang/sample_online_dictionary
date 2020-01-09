BIN=client
OBJ=main.o client.o
OBJ+=t_net.o menu.o
CC=gcc

$(BIN):$(OBJ)
	@$(CC) -o $(BIN) $(OBJ)
	@echo "编译完成！"
%.o:%.c
	@$(CC) -c -o $@ $<

clean:
	@rm $(BIN) $(OBJ)
	@echo "清理完成！"

