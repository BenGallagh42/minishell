# 🐚 minishell

> A fully functional UNIX shell built from scratch in C — because writing your own shell is the only way to truly understand one.

---

## 📖 Overview

**minishell** is a École 42 project that reimplements a simplified version of `bash`. It handles everything from tokenization and parsing to process execution, redirections, pipes, and signal management — all without using any forbidden functions.

Built with a teammate, the project covers the full lifecycle of a shell command: reading user input, lexing it into tokens, parsing into a command tree, expanding variables and wildcards, then executing with proper process management and error handling.

---

## ✨ Features

- **Interactive prompt** with readline history
- **Command execution** via `execve` with `$PATH` resolution
- **Pipes** — chain commands with `|`
- **Redirections** — `<`, `>`, `>>`, and heredoc `<<`
- **Variable expansion** — `$VAR`, `$?`
- **Wildcard expansion** — `*` globbing in the current directory
- **Quote handling** — single quotes `'` and double quotes `"`
- **Signals** — `Ctrl+C`, `Ctrl+D`, `Ctrl+\` behave like bash
- **Built-in commands** — implemented natively without forking

---

## 🔧 Built-ins

| Command | Description |
|---------|-------------|
| `echo` | Print arguments, supports `-n` flag |
| `cd` | Change directory, updates `PWD` and `OLDPWD` |
| `pwd` | Print current working directory |
| `export` | Set or display environment variables |
| `unset` | Remove environment variables |
| `env` | Print all environment variables |
| `exit` | Exit the shell with an optional status code |

---

## 🏗️ Architecture

```
minishell/
├── inc/              # Header — structs, enums, prototypes
├── libft/            # Custom C library (ft_printf, GNL, linked lists...)
└── src/
    ├── main.c        # Entry point, shell loop
    ├── shell_*.c     # Init, run, signals, cleanup
    ├── lexer/        # Tokenization
    ├── parser/       # AST construction, expansion, heredoc
    ├── exec/         # Process execution, redirections, path resolution
    └── built-ins/    # Native command implementations
```

### Pipeline

```
Input → Lexer → Tokens → Parser → Command tree → Executor → Output
                  ↓                     ↓
            Quote handling         Variable & wildcard expansion
            Dollar expansion       Heredoc processing
            Operator detection     Redirection setup
```

---

## 📁 Source Breakdown

### `lexer/`
Breaks raw input into a token list. Handles operators, words, quotes, `$` expansions, redirectors, and wildcards as distinct token types.

### `parser/`
Converts the token list into a structured command representation. Manages syntax validation, variable expansion, heredoc collection, redirection parsing, and wildcard resolution.

### `exec/`
Executes the parsed commands. Handles `fork`/`execve`, pipe chaining, file descriptor redirections, `$PATH` lookup, and process wait/status management.

### `built-ins/`
Each built-in is implemented natively inside the shell process (no fork). Includes full `cd` logic with `OLDPWD` tracking, proper `export` formatting, and `exit` with status propagation.

---

## 🚀 Usage

```bash
make
./minishell
```

```bash
minishell$ echo "Hello, $USER!"
Hello, ben!

minishell$ ls | grep .c | wc -l
42

minishell$ cat << EOF
> Hello
> World
> EOF

minishell$ export MY_VAR=42 && echo $MY_VAR
42

minishell$ exit
```

---

## 🛠️ Build

```bash
make        # Build minishell
make clean  # Remove object files
make fclean # Remove everything including binary
make re     # Rebuild from scratch
```

---

## 🧠 Concepts Covered

`Lexical analysis` · `Parsing` · `AST construction` · `Process management` · `fork / execve / waitpid` · `File descriptors` · `Pipes` · `Redirections` · `Heredoc` · `Signal handling` · `Environment variables` · `Wildcard globbing` · `Quote semantics` · `Memory management`

---

## 👥 Team

Built as a group project at **École 42** — pair programming, code reviews, and shared debugging sessions included.

---

*"Writing a shell taught me more about UNIX than any book ever could."*
