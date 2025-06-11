#include "minishell.h"


// Helper functions
t_token *create_token(t_token_type type, const char *value)
{
	t_token *token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = type;
	token->value = ft_strdup(value);
	token->next = NULL;
	return (token);
}

void free_tokens(t_token *tokens)
{
	t_token *tmp;

	while (tokens)
	{
		tmp = tokens;
		tokens = tokens->next;
		free(tmp->value);
		free(tmp);
	}
}

static t_token *tester_add_token(t_token **head, t_token_type type, const char *value)
{
	t_token *token;
	t_token *current;

	token = create_token(type, value);
	if (!token)
		return (NULL);
	if (!*head)
		*head = token;
	else
	{
		current = *head;
		while (current->next)
			current = current->next;
		current->next = token;
	}
	return (token);
}

t_token *create_token_list(int count, ...)
{
	va_list args;
	t_token *head;
	int i;
	t_token_type type;
	const char *value;

	va_start(args, count);
	head = NULL;
	i = 0;
	while (i < count)
	{
		type = va_arg(args, t_token_type);
		value = va_arg(args, const char *);
		if (!tester_add_token(&head, type, value))
		{
			free_tokens(head);
			va_end(args);
			return (NULL);
		}
		i++;
	}
	va_end(args);
	return (head);
}

void setup_minishell_env(t_program *minishell, int count, ...)
{
	va_list args;
	int i;
	const char *env;

	va_start(args, count);
	minishell->envp = malloc((count + 1) * sizeof(char*));
	i = 0;
	while (i < count)
	{
		env = va_arg(args, const char *);
		minishell->envp[i] = ft_strdup(env);
		i++;
	}
	minishell->envp[count] = NULL;
	va_end(args);
}

int assert_equal(const char *expected, const char *actual, const char *message)
{
	if (strcmp(expected, actual) == 0)
	{
		printf("[PASS] %s\n", message);
		return (1);
	}
	printf("[FAIL] %s: expected '%s', got '%s'\n", message, expected, actual);
	return (0);
}

int assert_int_equal(int expected, int actual, const char *message)
{
	if (expected == actual)
	{
		printf("[PASS] %s\n", message);
		return (1);
	}
	printf("[FAIL] %s: expected %d, got %d\n", message, expected, actual);
	return (0);
}

int assert_null(void *ptr, const char *message)
{
	if (ptr == NULL)
	{
		printf("[PASS] %s\n", message);
		return (1);
	}
	printf("[FAIL] %s: expected NULL, got %p\n", message, ptr);
	return (0);
}

// Test functions
void test_simple_command()
{
	t_program minishell;
	t_token *tokens1;
	t_token *tokens2;
	t_token *tokens3;
	t_command *cmd1;
	t_command *cmd2;
	t_command *cmd3;

	printf("Testing simple command...\n");
	minishell = (t_program){0};

	// Test 1: echo hello
	tokens1 = create_token_list(2, TKN_WORD, "echo", TKN_WORD, "hello");
	cmd1 = main_parser(tokens1, &minishell);
	if (cmd1 && cmd1->args && cmd1->args[0] && cmd1->args[1] && !cmd1->args[2])
	{
		assert_equal("echo", cmd1->args[0], "First arg (echo hello)");
		assert_equal("hello", cmd1->args[1], "Second arg (echo hello)");
	}
	else
		printf("[FAIL] Incorrect args for echo hello\n");
	free_command(cmd1);
	free_tokens(tokens1);

	// Test 2: echo hello world
	tokens2 = create_token_list(3, TKN_WORD, "echo", TKN_WORD, "hello", TKN_WORD, "world");
	cmd2 = main_parser(tokens2, &minishell);
	if (cmd2 && cmd2->args && cmd2->args[0] && cmd2->args[1] && cmd2->args[2] && !cmd2->args[3])
	{
		assert_equal("echo", cmd2->args[0], "First arg (echo hello world)");
		assert_equal("hello", cmd2->args[1], "Second arg (echo hello world)");
		assert_equal("world", cmd2->args[2], "Third arg (echo hello world)");
	}
	else
		printf("[FAIL] Incorrect args for echo hello world\n");
	free_command(cmd2);
	free_tokens(tokens2);

	// Test 3: echo "hello world"
	tokens3 = create_token_list(2, TKN_WORD, "echo", TKN_WORD, "\"hello world\"");
	cmd3 = main_parser(tokens3, &minishell);
	if (cmd3 && cmd3->args && cmd3->args[0] && cmd3->args[1] && !cmd3->args[2])
	{
		assert_equal("echo", cmd3->args[0], "First arg (echo \"hello world\")");
		assert_equal("hello world", cmd3->args[1], "Second arg (echo \"hello world\")");
	}
	else
		printf("[FAIL] Incorrect args for echo \"hello world\"\n");
	free_command(cmd3);
	free_tokens(tokens3);
}

void test_redirection(void)
{
	t_program minishell;
	t_token *tokens1;
	t_token *tokens2;
	t_token *tokens3;
	t_command *cmd1;
	t_command *cmd2;
	t_command *cmd3;
	FILE *temp_file;

	printf("Testing redirection...\n");
	minishell = (t_program){0};

	// Test 1: cat < input.txt
	temp_file = fopen("input.txt", "w");
	if (!temp_file)
	{
		printf("[FAIL] Could not create temporary file\n");
		return;
	}
	fprintf(temp_file, "This is a test file.\n");
	fclose(temp_file);
	tokens1 = create_token_list(3, TKN_WORD, "cat", TKN_REDIR_IN, "<", TKN_WORD, "input.txt");
	cmd1 = main_parser(tokens1, &minishell);
	if (cmd1 && cmd1->args && cmd1->args[0] && !cmd1->args[1])
		assert_equal("cat", cmd1->args[0], "Command arg (cat < input.txt)");
	if (cmd1->redirs && cmd1->redirs->type == TKN_REDIR_IN && cmd1->redirs->target)
		assert_equal("input.txt", cmd1->redirs->target, "Redirection target (cat < input.txt)");
	else
		printf("[FAIL] Incorrect redirection for cat < input.txt\n");
	free_command(cmd1);
	free_tokens(tokens1);
	remove("input.txt");

	// Test 2: echo hello > out.txt
	tokens2 = create_token_list(4, TKN_WORD, "echo", TKN_WORD, "hello", TKN_REDIR_OUT, ">", TKN_WORD, "out.txt");
	cmd2 = main_parser(tokens2, &minishell);
	if (cmd2 && cmd2->args && cmd2->args[0] && cmd2->args[1] && !cmd2->args[2])
	{
		assert_equal("echo", cmd2->args[0], "First arg (echo hello > out.txt)");
		assert_equal("hello", cmd2->args[1], "Second arg (echo hello > out.txt)");
	}
	if (cmd2->redirs && cmd2->redirs->type == TKN_REDIR_OUT && cmd2->redirs->target)
		assert_equal("out.txt", cmd2->redirs->target, "Redirection target (echo hello > out.txt)");
	else
		printf("[FAIL] Incorrect redirection for echo hello > out.txt\n");
	free_command(cmd2);
	free_tokens(tokens2);

	// Test 3: cat < nonexistent.txt
	tokens3 = create_token_list(3, TKN_WORD, "cat", TKN_REDIR_IN, "<", TKN_WORD, "nonexistent.txt");
	cmd3 = main_parser(tokens3, &minishell);
	assert_null(cmd3, "Invalid input redirection");
	assert_int_equal(1, minishell.error_code, "Error code for invalid input file");
	free_tokens(tokens3);
}

void test_pipeline()
{
	t_token *tokens;
	t_program minishell;
	t_command *cmd;
	t_command *second;

	printf("Testing pipeline...\n");
	tokens = create_token_list(5, TKN_WORD, "echo", TKN_WORD, "hello", TKN_PIPE, "|", TKN_WORD, "wc", TKN_WORD, "-l");
	minishell = (t_program){0};
	cmd = main_parser(tokens, &minishell);
	if (!cmd || !cmd->next)
	{
		printf("[FAIL] Parser did not return two commands\n");
		free_tokens(tokens);
		free_command(cmd);
		return;
	}
	if (cmd->args && cmd->args[0] && cmd->args[1] && !cmd->args[2])
	{
		assert_equal("echo", cmd->args[0], "First command first arg");
		assert_equal("hello", cmd->args[1], "First command second arg");
	}
	if (cmd->is_piped != 1)
		printf("[FAIL] First command should be piped\n");
	second = cmd->next;
	if (second->args && second->args[0] && second->args[1] && !second->args[2])
	{
		assert_equal("wc", second->args[0], "Second command first arg");
		assert_equal("-l", second->args[1], "Second command second arg");
	}
	free_command(cmd);
	free_tokens(tokens);
}

void test_heredoc()
{
	t_token *tokens;
	t_program minishell;
	t_command *cmd;

	printf("Testing heredoc...\n");
	tokens = create_token_list(3, TKN_WORD, "cat", TKN_REDIR_HEREDOC, "<<", TKN_WORD, "EOF");
	minishell = (t_program){0};
	cmd = main_parser(tokens, &minishell);
	if (!cmd)
	{
		printf("[FAIL] Parser returned NULL\n");
		free_tokens(tokens);
		return;
	}
	if (cmd->args && cmd->args[0] && !cmd->args[1])
		assert_equal("cat", cmd->args[0], "Command arg");
	if (cmd->redirs && cmd->redirs->type == TKN_REDIR_HEREDOC && cmd->redirs->target)
		assert_equal("EOF", cmd->redirs->target, "Heredoc delimiter");
	free_command(cmd);
	free_tokens(tokens);
}

void test_variable_expansion()
{
	t_program minishell;
	t_token *tokens1;
	t_token *tokens2;
	t_token *tokens3;
	t_token *tokens4;
	t_token *tokens5;
	t_command *cmd1;
	t_command *cmd2;
	t_command *cmd3;
	t_command *cmd4;
	t_command *cmd5;

	printf("Testing variable expansion...\n");
	minishell = (t_program){0};
	setup_minishell_env(&minishell, 3, "USER=testuser", "VAR1=hello", "VAR2=world");

	// Test 1: echo $USER
	tokens1 = create_token_list(2, TKN_WORD, "echo", TKN_WORD, "$USER");
	cmd1 = main_parser(tokens1, &minishell);
	if (cmd1 && cmd1->args && cmd1->args[0] && cmd1->args[1] && !cmd1->args[2])
	{
		assert_equal("echo", cmd1->args[0], "First arg (echo $USER)");
		assert_equal("testuser", cmd1->args[1], "Expanded variable (echo $USER)");
	}
	free_command(cmd1);
	free_tokens(tokens1);

	// Test 2: echo '$USER'
	tokens2 = create_token_list(2, TKN_WORD, "echo", TKN_WORD, "'$USER'");
	cmd2 = main_parser(tokens2, &minishell);
	if (cmd2 && cmd2->args && cmd2->args[0] && cmd2->args[1] && !cmd2->args[2])
	{
		assert_equal("echo", cmd2->args[0], "First arg (echo '$USER')");
		assert_equal("$USER", cmd2->args[1], "Unexpanded variable in single quotes");
	}
	free_command(cmd2);
	free_tokens(tokens2);

	// Test 3: echo "hello $USER"
	tokens3 = create_token_list(2, TKN_WORD, "echo", TKN_WORD, "\"hello $USER\"");
	cmd3 = main_parser(tokens3, &minishell);
	if (cmd3 && cmd3->args && cmd3->args[0] && cmd3->args[1] && !cmd3->args[2])
	{
		assert_equal("echo", cmd3->args[0], "First arg (echo \"hello $USER\")");
		assert_equal("hello testuser", cmd3->args[1], "Expanded variable in double quotes");
	}
	free_command(cmd3);
	free_tokens(tokens3);

	// Test 4: echo $UNSET_VAR
	tokens4 = create_token_list(2, TKN_WORD, "echo", TKN_WORD, "$UNSET_VAR");
	cmd4 = main_parser(tokens4, &minishell);
	if (cmd4 && cmd4->args && cmd4->args[0] && cmd4->args[1] && !cmd4->args[2])
	{
		assert_equal("echo", cmd4->args[0], "First arg (echo $UNSET_VAR)");
		assert_equal("", cmd4->args[1], "Undefined variable expands to empty string");
	}
	free_command(cmd4);
	free_tokens(tokens4);

	// Test 5: echo "$VAR1$VAR2"
	tokens5 = create_token_list(2, TKN_WORD, "echo", TKN_WORD, "\"$VAR1$VAR2\"");
	cmd5 = main_parser(tokens5, &minishell);
	if (cmd5 && cmd5->args && cmd5->args[0] && cmd5->args[1] && !cmd5->args[2])
	{
		assert_equal("echo", cmd5->args[0], "First arg (echo \"$VAR1$VAR2\")");
		assert_equal("helloworld", cmd5->args[1], "Concatenated variables");
	}
	free_command(cmd5);
	free_tokens(tokens5);

	ft_free_array(minishell.envp);
}

void test_syntax_error()
{
	t_program minishell;
	t_token *tokens1;
	t_token *tokens2;
	t_token *tokens3;
	t_token *tokens4;
	t_token *tokens5;
	t_command *cmd1;
	t_command *cmd2;
	t_command *cmd3;
	t_command *cmd4;
	t_command *cmd5;

	printf("Testing syntax error...\n");
	minishell = (t_program){0};

	// Test 1: Trailing pipe
	tokens1 = create_token_list(3, TKN_WORD, "echo", TKN_WORD, "hello", TKN_PIPE, "|");
	cmd1 = main_parser(tokens1, &minishell);
	assert_null(cmd1, "Trailing pipe");
	free_tokens(tokens1);

	// Test 2: Leading pipe
	tokens2 = create_token_list(3, TKN_PIPE, "|", TKN_WORD, "echo", TKN_WORD, "hello");
	cmd2 = main_parser(tokens2, &minishell);
	assert_null(cmd2, "Leading pipe");
	free_tokens(tokens2);

	// Test 3: Double pipe
	tokens3 = create_token_list(5, TKN_WORD, "echo", TKN_WORD, "hello", TKN_PIPE, "|", TKN_PIPE, "|", TKN_WORD, "wc");
	cmd3 = main_parser(tokens3, &minishell);
	assert_null(cmd3, "Double pipe");
	free_tokens(tokens3);

	// Test 4: Redirection without target
	tokens4 = create_token_list(2, TKN_WORD, "cmd", TKN_REDIR_OUT, ">");
	cmd4 = main_parser(tokens4, &minishell);
	assert_null(cmd4, "Redirection without target");
	free_tokens(tokens4);

	// Test 5: Redirection with missing command
	tokens5 = create_token_list(2, TKN_REDIR_IN, "<", TKN_WORD, "file");
	cmd5 = main_parser(tokens5, &minishell);
	assert_null(cmd5, "Redirection with missing command");
	free_tokens(tokens5);
}

void test_empty_input()
{
	t_program minishell;
	t_token *tokens2;
	t_command *cmd1;
	t_command *cmd2;

	printf("Testing empty input...\n");
	minishell = (t_program){0};
	cmd1 = main_parser(NULL, &minishell);
	assert_null(cmd1, "NULL tokens");
	assert_int_equal(0, minishell.error_code, "Error code for NULL tokens");

	tokens2 = create_token(TKN_WORD, "");
	cmd2 = main_parser(tokens2, &minishell);
	assert_null(cmd2, "Empty token value");
	assert_int_equal(0, minishell.error_code, "Error code for empty token");
	free_tokens(tokens2);
}

void test_long_inputs()
{
	t_program minishell;
	t_token *tokens;
	t_token *current;
	int i;
	const int num_args = 10;
	char arg[10];
	t_command *cmd;

	printf("Testing long inputs...\n");
	minishell = (t_program){0};
	tokens = create_token(TKN_WORD, "cmd");
	current = tokens;
	i = 1;
	while (i <= num_args)
	{
		sprintf(arg, "arg%d", i);
		current->next = create_token(TKN_WORD, arg);
		current = current->next;
		i++;
	}
	cmd = main_parser(tokens, &minishell);
	if (cmd && cmd->args && cmd->args[num_args])
	{
		assert_equal("cmd", cmd->args[0], "Command with many args");
		assert_equal("arg10", cmd->args[10], "Last arg");
	}
	else
		printf("[FAIL] Incorrect handling of long inputs\n");
	free_command(cmd);
	free_tokens(tokens);
}

void test_multiple_redirections()
{
	t_token *tokens;
	t_program minishell;
	t_command *cmd;

	printf("Testing multiple redirections...\n");
	tokens = create_token_list(5, TKN_WORD, "cmd", TKN_REDIR_OUT, ">", TKN_WORD, "out1", TKN_REDIR_OUT, ">", TKN_WORD, "out2");
	minishell = (t_program){0};
	cmd = main_parser(tokens, &minishell);
	if (cmd && cmd->redirs && cmd->redirs->next)
	{
		assert_int_equal(TKN_REDIR_OUT, cmd->redirs->type, "First redirection type");
		assert_equal("out1", cmd->redirs->target, "First redirection target");
		assert_int_equal(TKN_REDIR_OUT, cmd->redirs->next->type, "Second redirection type");
		assert_equal("out2", cmd->redirs->next->target, "Second redirection target");
	}
	else
		printf("[FAIL] Incorrect handling of multiple redirections\n");
	free_command(cmd);
	free_tokens(tokens);
}

void test_advanced_pipelines()
{
	t_token *tokens;
	t_program minishell;
	t_command *cmd;

	printf("Testing advanced pipelines...\n");
	tokens = create_token_list(7, TKN_WORD, "cmd1", TKN_PIPE, "|", TKN_WORD, "cmd2", TKN_PIPE, "|", TKN_WORD, "cmd3", TKN_REDIR_OUT, ">", TKN_WORD, "out");
	minishell = (t_program){0};
	cmd = main_parser(tokens, &minishell);
	if (!cmd || !cmd->next || !cmd->next->next)
	{
		printf("[FAIL] Parser did not return three commands\n");
	}
	else
	{
		assert_equal("cmd1", cmd->args[0], "First command");
		assert_int_equal(1, cmd->is_piped, "First command piped");
		assert_equal("cmd2", cmd->next->args[0], "Second command");
		assert_int_equal(1, cmd->next->is_piped, "Second command piped");
		assert_equal("cmd3", cmd->next->next->args[0], "Third command");
		assert_int_equal(0, cmd->next->next->is_piped, "Third command not piped");
		if (cmd->next->next->redirs && cmd->next->next->redirs->type == TKN_REDIR_OUT)
			assert_equal("out", cmd->next->next->redirs->target, "Redirection target");
		else
			printf("[FAIL] Incorrect redirection\n");
	}
	free_command(cmd);
	free_tokens(tokens);
}

int main()
{
	printf("Running parser tests...\n");
	test_simple_command();
	test_redirection();
	test_pipeline();
	test_heredoc();
	test_variable_expansion();
	test_syntax_error();
	test_empty_input();
	test_long_inputs();
	test_multiple_redirections();
	test_advanced_pipelines();
	printf("All tests completed.\n");
	return (0);
}
