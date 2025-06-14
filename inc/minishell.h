/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 18:16:10 by bboulmie          #+#    #+#             */
/*   Updated: 2025/06/11 19:37:42 by bboulmie         ###   ########.fr       */
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
}	t_token_type;

// LEXER: Main token struct
typedef struct s_token
{
	t_token_type	type;
	char			*value;
	int				error;
	struct s_token	*next;
}	t_token;

// PARSER: All parsing structs
typedef struct s_redirection
{
	t_token_type			type;
	char					*target;
	char					*content;
	struct s_redirection	*next;
}	t_redirection;

typedef struct s_command
{
	char					**args;
	t_redirection			*redirs;
	int						is_piped;
	int						is_background;
	int						is_builtin;
	struct s_command		*next;
}	t_command;

// MINISHELL: Main program struct
typedef struct s_program
{
	char			**envp;
	t_token			*token;
	char			*input_buffer;
	int				error_code;
	t_command		*cmd_list;
}					t_program;

// FUNCTIONS
// Lexer functions
t_token			*main_lexer(const char *input, t_program *minishell);
void			add_token(t_token **head, t_token **curr, t_token_type type,
					char *val);
void			token_dollar(const char **input, t_token **head,
					t_token **current);
void			token_operator(const char **input, t_token **head,
					t_token **current);
void			token_quotes(const char **input, t_token **head,
					t_token **current, t_program *minishell);
void			token_redirector_main(const char **input, t_token **head,
					t_token **current);
void			token_word(const char **input, t_token **head,
					t_token **current);
void			token_wildcard(const char **input, t_token **head,
					t_token **current);
int				has_wildcard(const char *input);

// Parser functions
int				check_syntax(t_token *tokens, t_program *minishell);
t_command		*main_parser(t_token *tokens, t_program *minishell);
t_command		*parse_pipeline(t_token **tokens, t_program *minishell);
t_command		*parse_simple_cmd(t_token **tokens, t_program *minishell);
t_redirection	*parse_redir(t_token **tokens, t_program *minishell);
int				handle_heredoc(t_redirection *redir, const char *delim,
					t_program *minishell);
int				validate_redir_target(t_redirection *redir,
					t_program *minishell);
void			print_redir_error(t_token *current);
char			*read_heredoc(const char *delimiter, t_program *minishell,
					int expand);
t_redirection	*list_to_redir_array(t_list *list);
char			**list_to_array(t_list *list);
void			expand_single_variable(const char **str, char **result,
					t_program *minishell);
void			free_command(t_command *cmd);
void			free_redirection(void *content);
void			*free_array_partial(char **array, size_t i);
void			free_redirs(t_redirection *redirs);
void			free_redirection(void *content);
void			ft_free_array(char **array);
void			free_cmd(t_command *cmd, t_list *args, t_list *redirs);
void			free_lists(t_list **args, t_list **redirs);
int				process_unquoted(const char **ptr, char **result,
					t_program *minishell);
char			*expand_and_remove_quotes(const char *str,
					t_program *minishell);
int				is_redirection_token(t_token_type type);
char			*remove_quotes(const char *str);
char			*ft_strjoin_free(char *s1, char *s2);
int				append_literal(char **result, const char *str, size_t len);
char			*ft_getenv(const char *name, char **envp);

// Exec functions

// Utility functions
void			ft_error(const char *error_msg);
#endif
