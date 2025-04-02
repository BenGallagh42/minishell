/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_putptr.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 16:29:15 by bboulmie          #+#    #+#             */
/*   Updated: 2025/03/04 17:54:06 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_ptrlen(uintptr_t nb)
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

static void	ft_ptrprint(uintptr_t nb)
{
	if (nb >= 16)
	{
		ft_ptrprint(nb / 16);
		ft_ptrprint(nb % 16);
	}
	else if (nb <= 9)
		ft_putchar(nb + '0');
	else
		ft_putchar(nb - 10 + 'a');
}

int	ft_putptr(uintptr_t ptr)
{
	int	count;

	count = 0;
	if (ptr == 0)
		return (write(1, "(nil)", 5));
	count += write(1, "0x", 2);
	ft_ptrprint(ptr);
	count += ft_ptrlen(ptr);
	return (count);
}
