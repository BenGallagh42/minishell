/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdelone_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/31 17:53:43 by bboulmie          #+#    #+#             */
/*   Updated: 2024/09/04 14:25:33 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstdelone(t_list *lst, void (*del)(void *))
{
	if (!lst)
		return ;
	(*del)(lst->content);
	free(lst);
}

// void	del(void *content)
// {
// 	free(content);
// }

// void	print_list(t_list *lst)
// {
// 	while (lst)
// 	{
// 		printf("Node content: '%s'\n", (char *)lst->content);
// 		lst = lst->next;
// 	}
// }

// int main(void)
// {
// 	t_list	*head = ft_lstnew(ft_strdup("Node 1"));
// 	t_list	*second = ft_lstnew(ft_strdup("Node 2"));
// 	t_list	*third = ft_lstnew(ft_strdup("Node 3"));
// 	t_list 	*temp;

// 	head->next = second;
// 	second->next = third;
// 	printf("List before:\n");
// 	print_list(head);
// 	temp = head->next;
// 	head->next = temp->next;
// 	ft_lstdelone(temp, del);
// 	printf("List after:\n");
// 	print_list(head);
// 	return (0);
// }