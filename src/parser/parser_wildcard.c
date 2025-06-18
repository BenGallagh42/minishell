/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_wildcard.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 16:49:15 by bboulmie          #+#    #+#             */
/*   Updated: 2025/06/18 16:49:20 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <dirent.h>
#include <stdlib.h>
#include <string.h>

static int	match_pattern(const char *pattern, const char *filename)
{
	const char	*p;
	const char	*f;
	const char	*star;

	p = NULL;
	f = NULL;
	star = NULL;
	p = pattern;
	f = filename;
	while (*f)
	{
		if (*p == '*')
		{
			star = p;
			p = p + 1;
			f = f + 1;
		}
		else if (*p == '?' || *p == *f)
		{
			p = p + 1;
			f = f + 1;
		}
		else if (star)
			f = f + 1;
		else
			return (0);
	}
	return (*p == '\0');
}

static t_list	*collect_matches(const char *pattern)
{
	DIR			*dir;
	struct dirent	*entry;
	t_list		*matches;

	dir = NULL;
	matches = NULL;
	dir = opendir(".");
	if (!dir)
		return (NULL);
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

static void	sort_matches(t_list *matches)
{
	t_list	*current;
	char	*temp;
	int		swapped;

	swapped = 0;
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

t_list	*expand_wildcard(const char *pattern)
{
	t_list	*matches;
	t_list	*result;

	matches = NULL;
	result = NULL;
	matches = collect_matches(pattern);
	if (!matches)
	{
		result = ft_lstnew(ft_strdup(pattern));
		return (result);
	}
	sort_matches(matches);
	return (matches);
}
