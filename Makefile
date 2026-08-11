CC = cc
CFLAGS = -Wall -Werror -Wextra
FOLDER_SOURCE = src/
FOLDER_BUILD = .build
FOLDER_INCLUDE = inc/

NAME = 
.PHONY: all clean fclean re

all: $(NAME)

clean:
	rm -rf $(FOLDER_BUILD)

fclean: clean
	rm -f $(NAME)

re: fclean all
