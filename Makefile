CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread
CPPFLAGS = -Iinc
FOLDER_BUILD = .build/

NAME = codexion
SOURCES = main.c \
		  src/init.c \
		  src/monitor.c \
		  src/parse.c \
		  src/sync.c \
		  src/threads.c \
		  src/time.c
OBJECTS = $(addprefix $(FOLDER_BUILD), $(SOURCES:.c=.o))

.PHONY: all clean fclean re

all: $(NAME)

$(NAME): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(NAME)

$(FOLDER_BUILD)%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

clean:
	rm -rf $(FOLDER_BUILD)

fclean: clean
	rm -f $(NAME)

re: fclean all
