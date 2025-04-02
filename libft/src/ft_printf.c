/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 13:35:00 by bboulmie          #+#    #+#             */
/*   Updated: 2025/03/04 17:54:54 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_specifier_check(va_list args, char spec)
{
	int	count;

	count = 0;
	if (spec == 'c')
		count += ft_putchar(va_arg(args, int));
	else if (spec == 's')
		count += ft_putstr(va_arg(args, char *));
	else if (spec == 'p')
		count += ft_putptr(va_arg(args, uintptr_t));
	else if (spec == 'd' || spec == 'i')
		count += ft_putnbr(va_arg(args, int));
	else if (spec == 'u')
		count += ft_putunsigned(va_arg(args, unsigned int));
	else if (spec == 'x' || spec == 'X')
		count += ft_puthex(va_arg(args, unsigned int), spec);
	else if (spec == '%')
		count += ft_putchar('%');
	return (count);
}

int	ft_printf(const char *string, ...)
{
	int		i;
	int		count;
	va_list	args;

	if (!string)
		return (0);
	va_start(args, string);
	count = 0;
	i = 0;
	while (string[i])
	{
		if (string[i] == '%')
		{
			i++;
			count += ft_specifier_check(args, string[i]);
		}
		else
			count += ft_putchar(string[i]);
		i++;
	}
	va_end(args);
	return (count);
}
