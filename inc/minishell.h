/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 18:16:10 by bboulmie          #+#    #+#             */
/*   Updated: 2025/06/03 18:20:14 by bboulmie         ###   ########.fr       */
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

// PARSER: All parsing structs
typedef struct s_redirection
{
	t_token_type			type;
	char					*target;
	struct s_redirection	*next;
}							t_redirection;

typedef struct s_command
{
	char			**args;
	t_redirection	*redirs;
	int				is_piped;
	int				is_background;
}					t_command;

typedef enum e_logical_op
{
	LOGICAL_NONE,
	LOGICAL_AND,
	LOGICAL_OR
}	t_logical_op;

typedef struct s_syntax_tree
{
	enum
	{
		TYPE_COMMAND,
		TYPE_PIPELINE,
		TYPE_LOGICAL
	}	e_type;
	union
	{
		t_command	*command;
		struct
		{
			t_command	**commands;
			int			cmd_count;
		}	s_pipeline;
		struct
		{
			struct s_syntax_tree	*left;
			struct s_syntax_tree	*right;
			t_logical_op			operator;
		}	s_logical;
	}	u_data;
}	t_syntax_tree;

// MINISHELL: Main program struct
typedef struct s_program
{
	char			**envp;
	t_token			*token;
	char			*input_buffer;
	int				error_code;
	t_syntax_tree	*ast;
}					t_program;

// FUNCTIONS
// Lexer functions
t_token			*main_lexer(const char *input, t_program *minishell);
void			add_token(t_token **head, t_token **curr, t_token_type type, char *val);
void			token_dollar(const char **input, t_token **head, t_token **current);
void			token_operator(const char **input, t_token **head, t_token **current);
void			token_quotes(const char **input, t_token **head,
					t_token **current, t_program *minishell);
void			token_redirector_main(const char **input, t_token **head,
					t_token **current);
void			token_word(const char **input, t_token **head, t_token **current);
void			token_wildcard(const char **input, t_token **head, t_token **current);
int				has_wildcard(const char *input);

// Parser functions
t_syntax_tree	*parse_tokens(t_token *tokens, t_program *minishell);
t_command		*parse_simple_cmd(t_token **tokens, t_program *minishell);
t_syntax_tree	*build_pipeline_node(t_command **cmds, int cmd_count);
t_redirection	*list_to_redir_array(t_list *list);
char			**list_to_array(t_list *list);
void			free_syntax_tree(t_syntax_tree *node);
void			free_command(t_command *cmd);
void			free_redirection(void *content);

// Exec functions


// Utility functions
void			ft_error(const char *error_msg);
#endif