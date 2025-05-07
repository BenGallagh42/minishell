/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 17:05:05 by bboulmie          #+#    #+#             */
/*   Updated: 2025/04/11 17:07:34 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	ft_error(const char *error_msg)
{
	ft_putstr_fd((char *)error_msg, STDERR_FILENO);
	exit(EXIT_FAILURE);
}
