NAME1 = server
NAME2 = client

CC = gcc

CFLAGS =  -Wall -Werror -Wextra

OBJ1 = server.o errorMsg.o
OBJ2 = client.o errorMsg.o

HEADER = serClient.h


all : $(NAME1) $(NAME2)

$(NAME1) : $(OBJ1)
	@$(CC)  $^ $(CFLAGS) -o $@

$(NAME2) : $(OBJ2)
	@$(CC)  $^ $(CFLAGS) -o $@

clean:
	@rm -rf $(OBJ1)
	@rm -rf $(OBJ2)
	
fclean: clean
	@rm -rf $(NAME1)
	@rm -rf $(NAME2)
	
re: fclean all
