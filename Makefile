NAME		= codexion
CC			= cc
HEADER_DIR	= headers
SRC_DIR		= src

CFLAGS		= -Wall -Wextra -Werror -pthread -I$(HEADER_DIR) -g

SRC		= $(addprefix $(SRC_DIR)/,\
			checker.c\
			cleaner.c\
			dongle_manager.c\
			heap.c\
			init.c\
			main.c\
			monitor.c\
			routine.c\
			utils.c\
			timer.c)

OBJ		= $(SRC:.c=.o)
HEADERS		= $(addprefix $(HEADER_DIR)/,)
INCLUDES	= -I$(HEADER_DIR)

SAN_CFLAGS	= -Wall -Wextra -pthread -I$(HEADER_DIR) -g3 -O1 -fno-omit-frame-pointer

TSAN_FLAGS	= $(SAN_CFLAGS) -fsanitize=thread
ASAN_FLAGS	= $(SAN_CFLAGS) -fsanitize=address,leak
UBSAN_FLAGS	= $(SAN_CFLAGS) -fsanitize=undefined

all: $(NAME)

$(NAME): $(OBJ) Makefile
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

tsan: $(SRC) Makefile
	$(CC) $(TSAN_FLAGS) -o $(NAME)_tsan $(SRC)
	@echo "[✓] $(NAME)_tsan prêt"

asan: $(SRC) Makefile
	$(CC) $(ASAN_FLAGS) -o $(NAME)_asan $(SRC)
	@echo "[✓] $(NAME)_asan prêt"

ubsan: $(SRC) Makefile
	$(CC) $(UBSAN_FLAGS) -o $(NAME)_ubsan $(SRC)
	@echo "[✓] $(NAME)_ubsan prêt"

san: tsan asan ubsan

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME) $(NAME)_tsan $(NAME)_asan $(NAME)_ubsan

re: fclean all

.PHONY: all clean fclean re tsan asan ubsan san