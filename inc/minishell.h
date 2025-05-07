/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 18:16:10 by bboulmie          #+#    #+#             */
/*   Updated: 2025/05/07 17:25:16 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

// LIBS
# include "../libft/inc/libft.h"
# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <termios.h>
# include <unistd.h>

// MACROS


// STRUCTS
// LEXER: Token type enum struct
typedef enum e_token_type
{
	TKN_WORD,
	TKN_ENV,
	TKN_STATUS,
	TKN_EOF,
	TKN_BG,
	TKN_REDIR_IN,
	TKN_REDIR_OUT,
	TKN_REDIR_HEREDOC,
	TKN_REDIR_APPEND,
	TKN_SINGLE_QUOTES,
	TKN_DOUBLE_QUOTES,
	TKN_PIPE,
	TKN_START,
	TKN_END,
	TKN_AND,
	TKN_OR,
	TKN_WILDCARD,
	TKN_ASSIGN_VAR
}					t_token_type;

// LEXER: Main token struct
typedef struct s_token
{
	t_token_type	type;
	char			*value;
	int				error;
	struct s_token	*next;
}					t_token;

// MINISHELL: Main program struct
typedef struct s_program
{
	char			**envp;
	t_token			*token;
	char			*input_buffer;
	int				error_code;	
}					t_program;

// FUNCTIONS
// Lexer functions
t_token	*main_lexer(const char *input, t_program *minishell);
void	add_token(t_token **head, t_token **curr, t_token_type type, char *val);
void	token_dollar(const char **input, t_token **head, t_token **current);
void	token_operator(const char **input, t_token **head, t_token **current);
void	token_quotes(const char **input, t_token **head,
			t_token **current, t_program *minishell);
void	token_redirector_main(const char **input, t_token **head,
			t_token **current);
void	token_word(const char **input, t_token **head, t_token **current);
void	token_wildcard(const char **input, t_token **head, t_token **current);

// Parser functions

// Exec functions

#endif