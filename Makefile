# Makefile for Lexer Tester

# Variables
NAME = parser_tester
CC = cc
CFLAGS = -Wall -Wextra -Werror -g
INC = -Iinc -Ilibft/inc
LIBFT = libft/libft.a

# Directories
SRC_DIR = src
TEST_DIR = tests
OBJ_DIR = obj

# Source Files
SRC_FILES = $(SRC_DIR)/lexer/lexer_utils.c \
            $(SRC_DIR)/lexer/lexer.c \
            $(SRC_DIR)/lexer/token_dollar.c \
            $(SRC_DIR)/lexer/token_operator.c \
            $(SRC_DIR)/lexer/token_quotes.c \
            $(SRC_DIR)/lexer/token_redirector.c \
			$(SRC_DIR)/lexer/token_wildcard.c \
            $(SRC_DIR)/lexer/token_word.c \
			$(SRC_DIR)/parser/parser.c \
			$(SRC_DIR)/parser/parser_utils.c \
			$(SRC_DIR)/parser/parser_command.c \
			$(SRC_DIR)/parser/parser_cleanup.c

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