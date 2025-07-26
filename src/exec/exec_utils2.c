/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnithyan <hnithyan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 21:33:33 by hnithyan          #+#    #+#             */
/*   Updated: 2025/07/26 21:36:10 by hnithyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	generate_tmp_filename(char *buffer, int count)
{
	char	*base;
	char	*num;
	int		i;

	base = "/tmp/minishell_heredoc_";
	i = 0;
	while (base[i])
	{
		buffer[i] = base[i];
		i++;
	}
	num = ft_itoa(count);
	if (!num)
		return ;
	while (*num)
	{
		buffer[i] = *num++;
		i++;
	}
	buffer[i] = '\0';
}
