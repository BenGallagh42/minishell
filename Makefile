# Minishell Makefile

# Variables
NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror -g -O0
INC = -Iinc -Ilibft/inc
LIBFT = libft/libft.a
DEPFLAGS = -MMD -MP

# Directories
LEXER_DIR = src/lexer
PARSER_DIR = src/parser
MAIN_DIR = src
OBJ_DIR = obj

# Source Files
LEXER_SRC = $(LEXER_DIR)/lexer.c \
            $(LEXER_DIR)/lexer_utils.c \
            $(LEXER_DIR)/token_dollar.c \
            $(LEXER_DIR)/token_operator.c \
            $(LEXER_DIR)/token_quotes.c \
            $(LEXER_DIR)/token_redirector.c \
            $(LEXER_DIR)/token_wildcard.c \
            $(LEXER_DIR)/token_word.c 

PARSER_SRC = $(PARSER_DIR)/parser.c \
             $(PARSER_DIR)/parser_command.c \
             $(PARSER_DIR)/parser_redir.c \
             $(PARSER_DIR)/parser_redir_utils.c \
             $(PARSER_DIR)/parser_heredoc.c \
             $(PARSER_DIR)/parser_expand.c \
             $(PARSER_DIR)/parser_utils.c \
             $(PARSER_DIR)/parser_utils2.c \
             $(PARSER_DIR)/parser_utils3.c \
             $(PARSER_DIR)/parser_syntax.c \
             $(PARSER_DIR)/parser_cleanup.c 

MAIN_SRC = $(MAIN_DIR)/main.c

# Object Files
SRC = $(LEXER_SRC) $(PARSER_SRC) $(MAIN_SRC)
OBJ_FILES = $(patsubst %.c,$(OBJ_DIR)/%.o,$(notdir $(SRC)))
DEPS = $(OBJ_FILES:.o=.d)

# VPATH
VPATH = $(LEXER_DIR):$(PARSER_DIR):$(MAIN_DIR)

# Colors
GREEN = \033[1;32m
YELLOW = \033[1;33m
BLUE = \033[1;34m
RESET = \033[0m

# Rules
all: $(NAME)

$(NAME): $(OBJ_FILES) $(LIBFT)
	@echo "$(BLUE)✨ Linking $@...$(RESET)"
	@$(CC) $(CFLAGS) $(INC) -o $@ $(OBJ_FILES) $(LIBFT) -lreadline
	@echo "$(GREEN)🚀 Successfully built $@$(RESET)"

$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	@echo "$(BLUE)✨ Compiling $<...$(RESET)"
	@$(CC) $(CFLAGS) $(INC) $(DEPFLAGS) -c $< -o $@

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(LIBFT):
	@echo "$(BLUE)✨ Building libft...$(RESET)"
	@$(MAKE) --no-print-directory -C libft > /dev/null

clean:
	@echo "$(YELLOW)💥 Cleaning object files...$(RESET)"
	@rm -rf $(OBJ_DIR)
	@echo "$(YELLOW)💥 Cleaning libft objects...$(RESET)"
	@$(MAKE) --no-print-directory -C libft clean > /dev/null

fclean: clean
	@echo "$(YELLOW)💥 Removing $(NAME)...$(RESET)"
	@rm -f $(NAME)
	@echo "$(YELLOW)💥 Cleaning libft...$(RESET)"
	@$(MAKE) --no-print-directory -C libft fclean > /dev/null

re: fclean all

# Dependencies
-include $(DEPS)

.PHONY: all clean fclean re