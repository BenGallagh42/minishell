/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_putnbr.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 15:17:59 by bboulmie          #+#    #+#             */
/*   Updated: 2025/03/04 18:14:32 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_putnbr(int n)
{
	int		count;
	char	*str;

	str = ft_itoa(n);
	if (!str)
		return (0);
	count = ft_putstr(str);
	free(str);
	return (count);
}
