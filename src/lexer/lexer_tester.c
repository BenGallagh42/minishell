#include "../../inc/minishell.h"

// To test and debug the lexer, should not be uploaded to the repository

static const char *token_type_names[] = {
	"TKN_WORD",
	"TKN_ENV",
	"TKN_STATUS",
	"TKN_EOF",
	"TKN_BG",
	"TKN_REDIR_IN",
	"TKN_REDIR_OUT",
	"TKN_REDIR_HEREDOC",
	"TKN_REDIR_APPEND",
	"TKN_SINGLE_QUOTES",
	"TKN_DOUBLE_QUOTES",
	"TKN_PIPE",
	"TKN_START",
	"TKN_END",
	"TKN_AND",
	"TKN_OR",
	"TKN_WILDCARD",
	"TKN_ASSIGN_VAR"
};

t_program	*create_test_minishell(const char *input)
{
	t_program	*minishell;

	minishell = malloc(sizeof(t_program));
	if (minishell)
	{
		minishell->input_buffer = ft_strdup(input);
	}
	return (minishell);
}

static void print_tokens(t_token *tokens)
{
	while (tokens)
	{
		printf("Type: %d, Token: %s, Value: '%s'\n", tokens->type, token_type_names[tokens->type], tokens->value);
		tokens = tokens->next;
	}
}

static void lexer_tester(t_program *minishell)
{
	t_token *tokens;

	tokens = main_lexer(minishell->input_buffer, minishell);
	print_tokens(tokens);
}

void	lexer_tester_program(const char *input)
{
	t_program	*minishell;

	minishell = create_test_minishell(input);
	if (!minishell)
	{
		perror("Failed to create minishell");
		return ;
	}
	lexer_tester(minishell);
	free(minishell->input_buffer);
	free(minishell);
}

int main(void)
{
	char *input;

	while (1)
	{
		input = readline("Enter input to tokenize: ");
		if (!input)
		{
			perror("Failed to read input\n");
			return (1);
		}
		lexer_tester_program(input);
		free(input);
	}
	return (0);
}
