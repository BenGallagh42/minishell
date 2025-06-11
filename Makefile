# Makefile for Parser Tester

# Variables
NAME = parser_tester
CC = cc
CFLAGS = -Wall -Wextra -Werror -g -O0
INC = -Iinc -Ilibft/inc
LIBFT = libft/libft.a

# Directories
SRC_DIR = src/parser
OBJ_DIR = obj

# Source Files
PARSER_SRC =	$(SRC_DIR)/parser.c \
            	$(SRC_DIR)/parser_command.c \
            	$(SRC_DIR)/parser_redir.c \
				$(SRC_DIR)/parser_redir_utils.c \
            	$(SRC_DIR)/parser_heredoc.c \
            	$(SRC_DIR)/parser_expand.c \
            	$(SRC_DIR)/parser_utils.c \
            	$(SRC_DIR)/parser_utils2.c \
				$(SRC_DIR)/parser_utils3.c \
            	$(SRC_DIR)/parser_syntax.c \
            	$(SRC_DIR)/parser_cleanup.c \
				$(SRC_DIR)/parser_tester.c

# Object Files
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(PARSER_SRC))

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