/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmagalha <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/26 12:33:20 by pmagalha          #+#    #+#             */
/*   Updated: 2023/04/26 13:01:50 by pmagalha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*testnode;

	if (*lst == NULL)
		*lst = new;
	else
	{
		testnode = *lst;
		while (testnode -> next != NULL)
			testnode = testnode->next;
		testnode->next = new;
	}
}
