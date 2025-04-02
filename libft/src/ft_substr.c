/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 10:55:45 by bboulmie          #+#    #+#             */
/*   Updated: 2025/03/04 17:35:37 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*new_str;
	size_t	slen;
	size_t	str_len;

	if (!s)
		return (NULL);
	slen = ft_strlen(s);
	if (start >= slen)
	{
		new_str = ft_calloc(1, sizeof(char));
		return (new_str);
	}
	str_len = slen - start;
	if (str_len > len)
		str_len = len;
	new_str = malloc(sizeof(char) * (str_len + 1));
	if (!new_str)
		return (NULL);
	ft_strlcpy(new_str, s + start, str_len + 1);
	return (new_str);
}
