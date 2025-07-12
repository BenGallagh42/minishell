/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 18:16:10 by bboulmie          #+#    #+#             */
/*   Updated: 2025/07/10 18:56:31 by bboulmie         ###   ########.fr       */
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

enum e_error_code
{
	ERR_NO_COMMAND = 1,
	ERR_SYNTAX_TOKEN,
	ERR_SYNTAX_PIPE,
	ERR_FILE_NOT_FOUND,
	ERR_PERMISSION_DENIED,
	ERR_MEMORY,
	ERR_INTERRUPTED,
	ERR_FORK,
	ERR_PIPE
};

// MINISHELL: Main program struct
typedef struct s_program
{
	char			**envp;
	t_token			*token;
	char			*input_buffer;
	int				error_code;
	t_command		*cmd_list;
}					t_program;

// MAIN FUNCTIONS
// Shell functions
void			init_shell(t_program *minishell, char **envp);
void			run_shell(t_program *minishell);
void			free_shell(t_program *minishell);

// Utils functions
void			free_tokens(t_token *tokens);
void			print_tokens(t_token *tokens);
void			print_redirections(t_redirection *redirs);
void			print_commands(t_command *cmd_list);

// LEXER FUNCTIONS
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

// PARSER FUNCTIONS
// Syntax Checking
// Check token syntax and validity
int				check_syntax(t_token *tokens, t_program *minishell);
int				process_token(t_token **current, int *has_command,
					t_program *minishell);
void			print_syntax_error(const char *token_value,
					t_program *minishell);
int				has_token_errors(t_token *tokens);

// Parsing Commands
// Handle main parsing logic for commands and pipelines
t_command		*main_parser(t_token *tokens, t_program *minishell);
t_command		*parse_pipeline(t_token **tokens, t_program *minishell);
t_command		*parse_simple_cmd(t_token **tokens, t_program *minishell);

// Redirection Handling
// Manage redirections, heredocs, and validation
t_redirection	*parse_redir(t_token **tokens, t_program *minishell);
int				handle_heredoc(t_redirection *redir, const char *delim,
					t_program *minishell);
int				validate_redir_target(t_redirection *redir,
					t_program *minishell);
char			*read_heredoc(const char *delimiter, t_program *minishell,
					int expand);
t_redirection	*list_to_redir_array(t_list *list);

// Expansion and Variable Handling
// Expand variables, wildcards, and remove quotes
void			expand_single_variable(const char **str, char **result,
					t_program *minishell);
int				process_unquoted(const char **ptr, char **result,
					t_program *minishell);
char			*expand_and_remove_quotes(const char *str,
					t_program *minishell);
char			*remove_quotes(const char *str);
char			*ft_getenv(const char *name, char **envp);
t_list			*expand_wildcard(const char *pattern, t_program *minishell);
t_list			*get_expanded_list(t_token *token, t_program *minishell);
void			append_expanded(t_list **args, t_list *to_add);

// Utility Functions
// Support list conversions, string ops, and token checks
char			**list_to_array(t_list *list);
int				append_literal(char **result, const char *str, size_t len);
char			*ft_strjoin_free(char *s1, char *s2);
int				is_redirection_token(t_token_type type);
int				is_builtin(const char *cmd);
void			init_command(t_command *cmd);
void			print_error_message(int error_code, const char *token_value,
					t_program *minishell);
int				validate_file_arg(const char *arg, t_program *minishell);
int				needs_file_validation(const char *cmd);

// Memory Management
// Free allocated memory for commands and redirections
void			free_command(t_command *cmd);
void			free_redirection(void *content);
void			*free_array_partial(char **array, size_t i);
void			free_redirs(t_redirection *redirs);
void			ft_free_array(char **array);
void			free_cmd(t_command *cmd, t_list *args, t_list *redirs);
void			free_lists(t_list **args, t_list **redirs);

//EXECUTION FUNCTIONS
// Handle main executioner logic
void			execute_commands(t_command *cmd, t_program *minishell);

// Handle builtins
void			execute_builtin(t_command *cmd, t_program *minishell);

// Built-ins
int				ft_echo(t_command *command);
int				ft_pwd(void);
int				ft_env(t_command *cmd, t_program *minishell);
int				ft_cd(t_command *cmd, t_program *minishell);

// Built-ins utils
int				ft_setenv(const char *name, const char *value,
					t_program *minishell);

// Handle redirections
void			handle_redirections(t_redirection *redirs,
					t_program *minishell);

// Utils
char			*find_command_path(char *cmd, t_program *minishell);

#endif
