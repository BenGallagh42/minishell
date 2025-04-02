/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/25 15:24:45 by bboulmie          #+#    #+#             */
/*   Updated: 2025/03/04 17:33:20 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *s, const char *to_find, size_t len)
{
	size_t	i;
	size_t	j;

	if (!s || !to_find)
		return (NULL);
	if (!to_find[0])
		return ((char *)s);
	i = 0;
	while (s[i] && i < len)
	{
		j = 0;
		while (s[i + j] == to_find[j] && (i + j) < len)
		{
			if (!to_find[j + 1])
				return ((char *)&s[i]);
			j++;
		}
		i++;
	}
	return (NULL);
}
