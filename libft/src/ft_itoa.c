/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 14:46:23 by bboulmie          #+#    #+#             */
/*   Updated: 2025/03/04 17:00:12 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	ft_len(int n)
{
	size_t	len;

	len = 0;
	if (n <= 0)
		len = 1;
	while (n != 0)
	{
		len++;
		n = n / 10;
	}
	return (len);
}

static char	*ft_nbrtochar(char *s, size_t nbr, size_t len)
{
	while (nbr > 0)
	{
		s[len] = '0' + (nbr % 10);
		nbr = nbr / 10;
		len--;
	}
	return (s);
}

char	*ft_itoa(int n)
{
	char	*s;
	size_t	len;
	long	nbr;

	nbr = n;
	len = ft_len(n);
	s = malloc(sizeof(char) * (len + 1));
	if (!s)
		return (NULL);
	s[len] = '\0';
	if (n == 0)
		s[0] = '0';
	if (n < 0)
	{
		s[0] = '-';
		nbr = -nbr;
	}
	if (n != 0)
		s = ft_nbrtochar(s, (size_t)nbr, len - 1);
	return (s);
}
