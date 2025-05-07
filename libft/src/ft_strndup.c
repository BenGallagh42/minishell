/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 16:00:36 by bboulmie          #+#    #+#             */
/*   Updated: 2025/04/22 16:01:46 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strndup(const char *s, size_t n)
{
	size_t	size;
	char	*dest;

	if (!s)
		return (NULL);
	size = ft_strlen(s);
	if (n < size)
		size = n;
	dest = (char *)malloc(sizeof(char) * (size + 1));
	if (!dest)
		return (NULL);
	ft_memcpy(dest, s, size);
	dest[size] = '\0';
	return (dest);
}
