/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_wildcard.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 18:48:48 by bboulmie          #+#    #+#             */
/*   Updated: 2025/07/29 18:48:50 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Sorts filenames in alphabetical order
static void	sort_matches(t_list *matches)
{
	t_list	*current;
	char	*temp;
	int		swapped;

	if (!matches || !matches->next)
		return ;
	swapped = 1;
	while (swapped)
	{
		swapped = 0;
		current = matches;
		while (current->next)
		{
			if (strcmp(current->content, current->next->content) > 0)
			{
				temp = current->content;
				current->content = current->next->content;
				current->next->content = temp;
				swapped = 1;
			}
			current = current->next;
		}
	}
}

// Helps match a wildcard pattern to a filename
static int	match_wildcard_loop(const char **p, const char **f,
	const char **star, const char **f_star)
{
	while (**f)
	{
		if (**p == '*')
		{
			*star = *p;
			(*p)++;
			*f_star = *f;
		}
		else if (**p == '?' || **p == **f)
		{
			(*p)++;
			(*f)++;
		}
		else if (*star)
		{
			*p = *star + 1;
			*f = ++(*f_star);
		}
		else
			return (0);
	}
	return (1);
}

// Checks if a wildcard pattern matches a filename
static int	match_pattern(const char *pattern, const char *filename)
{
	const char	*p;
	const char	*f;
	const char	*star;
	const char	*f_star;

	p = pattern;
	f = filename;
	star = NULL;
	f_star = NULL;
	if (!match_wildcard_loop(&p, &f, &star, &f_star))
		return (0);
	while (*p == '*')
		p++;
	return (*p == '\0');
}

// Finds filenames in the current directory that match a pattern
static t_list	*collect_matches(const char *pattern, t_program *minishell)
{
	DIR				*dir;
	struct dirent	*entry;
	t_list			*matches;

	dir = NULL;
	entry = NULL;
	matches = NULL;
	dir = opendir(".");
	if (!dir)
	{
		print_error_message(ERR_FILE_NOT_FOUND, ".", minishell);
		return (NULL);
	}
	entry = readdir(dir);
	while (entry != NULL)
	{
		if (match_pattern(pattern, entry->d_name))
			ft_lstadd_back(&matches, ft_lstnew(ft_strdup(entry->d_name)));
		entry = readdir(dir);
	}
	closedir(dir);
	return (matches);
}

// Turns a wildcard pattern into a sorted list of matching filenames
t_list	*expand_wildcard(const char *pattern, t_program *minishell)
{
	t_list	*matches;

	matches = collect_matches(pattern, minishell);
	if (!matches)
		return (ft_lstnew(ft_strdup(pattern)));
	sort_matches(matches);
	return (matches);
}
