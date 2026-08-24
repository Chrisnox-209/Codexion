CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread
CPPFLAGS = -Iinc
FOLDER_BUILD = .build/

NAME = codexion
SOURCES = main.c \
		  src/cleanup.c \
		  src/dongle.c \
		  src/dongle_init.c \
		  src/dongle_wait.c \
		  src/heap.c \
		  src/heap_delete.c \
		  src/heap_remove.c \
		  src/init.c \
		  src/monitor.c \
		  src/parse.c \
		  src/run.c \
		  src/sync.c \
		  src/threads.c \
		  src/time.c
OBJECTS = $(addprefix $(FOLDER_BUILD), $(SOURCES:.c=.o))
HEADERS = inc/codexion.h inc/heap.h inc/parsing.h inc/structures.h

.PHONY: all clean fclean re

all: $(NAME)

$(NAME): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(NAME)

$(FOLDER_BUILD)%.o: %.c $(HEADERS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

clean:
	rm -rf $(FOLDER_BUILD)

fclean: clean
	rm -f $(NAME)

re: fclean all
