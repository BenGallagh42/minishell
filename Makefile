# Makefile for Lexer Tester

# Variables
NAME = lexer_tester
CC = cc
CFLAGS = -Wall -Wextra -Werror -g
INC = -Iinc -Ilibft/inc
LIBFT = libft/libft.a

# Directories
SRC_DIR = src/lexer
TEST_DIR = tests
OBJ_DIR = obj

# Source Files
SRC_FILES = $(SRC_DIR)/lexer_tester.c \
            $(SRC_DIR)/lexer_utils.c \
            $(SRC_DIR)/lexer.c \
            $(SRC_DIR)/token_dollar.c \
            $(SRC_DIR)/token_operator.c \
            $(SRC_DIR)/token_quotes.c \
            $(SRC_DIR)/token_redirector.c \
			$(SRC_DIR)/token_wildcard.c \
            $(SRC_DIR)/token_word.c

# Object Files
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC_FILES))

# Rules
all: $(NAME)

$(NAME): $(OBJ_FILES) $(LIBFT)
	$(CC) $(CFLAGS) $(INC) -o $@ $^ -lreadline

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

$(LIBFT):
	$(MAKE) -C libft

clean:
	rm -rf $(OBJ_DIR)
	$(MAKE) -C libft clean

fclean: clean
	rm -f $(NAME)
	$(MAKE) -C libft fclean

re: fclean all

.PHONY: all clean fclean re