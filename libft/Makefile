# Sources de libft (tous les fichiers)
SRCS	=	ft_atoi.c ft_bzero.c ft_calloc.c ft_isalnum.c ft_isalpha.c ft_isascii.c ft_isdigit.c \
			ft_isprint.c ft_isspace.c ft_itoa.c ft_memchr.c ft_memcmp.c ft_memcpy.c ft_memmove.c ft_memset.c \
			ft_putchar_fd.c ft_putendl_fd.c ft_putnbr_fd.c ft_putstr_fd.c ft_split.c ft_strchr.c \
			ft_strcmp.c ft_strdup.c ft_striteri.c ft_strjoin.c ft_strlcat.c ft_strlcpy.c ft_strlen.c ft_strmapi.c \
			ft_strncmp.c ft_strndup.c ft_strnstr.c ft_strrchr.c ft_strtrim.c ft_substr.c ft_tolower.c ft_toupper.c \
			get_next_line.c ft_printf.c ft_printf_putchar.c ft_printf_puthex.c ft_printf_putnbr.c \
			ft_printf_putptr.c ft_printf_putstr.c ft_printf_putunsigned.c \
			ft_lstadd_back.c ft_lstadd_front.c ft_lstclear.c ft_lstdelone.c ft_lstiter.c \
			ft_lstlast.c ft_lstmap.c ft_lstnew.c ft_lstsize.c

# Dossier pour les fichiers objets
OBJ_DIR	=	obj

# Objets générés dans le dossier obj
OBJS	=	$(addprefix $(OBJ_DIR)/, ${SRCS:.c=.o})

# Nom de la bibliothèque
NAME	=	libft.a

# Commandes
LIBC	=	ar rcs
CC		=	cc
RM		=	rm -rf
CFLAGS	=	-Wall -Wextra -Werror -Iinc -g3 -O0

# Couleurs ANSI
GREEN	=	\033[0;32m
BLUE	=	\033[0;34m
RED		=	\033[0;31m
NC		=	\033[0m # No Color

# Règle de compilation des fichiers .c en .o
$(OBJ_DIR)/%.o: src/%.c
			@mkdir -p $(OBJ_DIR)
			@${CC} ${CFLAGS} -c $< -o $@

# Règle pour créer la bibliothèque avec tous les fichiers
$(NAME):	${OBJS}
			@${LIBC} ${NAME} ${OBJS}
			@echo "$(GREEN)Libft: Compiled 😊${NC}"

# Règle par défaut : compile tous les fichiers
all:		${NAME}

# Règle pour nettoyer les fichiers objets et le dossier obj
clean:
			@${RM} ${OBJ_DIR}
			@echo "$(RED)Libft: Objects cleaned 🧹${NC}"

# Règle pour nettoyer tout (objets + bibliothèque)
fclean:		clean
			@${RM} ${NAME}
			@echo "$(RED)Libft: Fully cleaned 🗑️${NC}"

# Règle pour tout recompiler
re:			fclean all
			@echo "$(GREEN)Libft: Recompiled 🔄${NC}"

# Déclarer les cibles comme non étant des fichiers
.PHONY:		all clean fclean re