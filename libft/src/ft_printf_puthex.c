/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_puthex.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 16:29:15 by bboulmie          #+#    #+#             */
/*   Updated: 2025/03/04 17:53:20 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_hexlen(unsigned int nb)
{
	int	len;

	len = 0;
	if (nb == 0)
		return (1);
	while (nb != 0)
	{
		len++;
		nb = nb / 16;
	}
	return (len);
}

static void	ft_hexprint(unsigned int nb, char spec)
{
	if (nb >= 16)
	{
		ft_hexprint(nb / 16, spec);
		ft_hexprint(nb % 16, spec);
	}
	else if (nb <= 9)
		ft_putchar(nb + '0');
	else if (spec == 'x')
		ft_putchar(nb - 10 + 'a');
	else if (spec == 'X')
		ft_putchar(nb - 10 + 'A');
}

int	ft_puthex(unsigned int nb, char spec)
{
	int	count;

	count = ft_hexlen(nb);
	if (nb == 0)
		return (write(1, "0", 1));
	ft_hexprint(nb, spec);
	return (count);
}
