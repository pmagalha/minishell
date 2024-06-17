/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-ppe <joao-ppe@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 17:30:52 by pmagalha          #+#    #+#             */
/*   Updated: 2024/06/17 18:01:25 by joao-ppe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_env_list	*create_dup(t_env_list *env_list)
{
	char		*dup_key;
	char		*dup_value;
	t_env_list	*dup_env;
	t_env_list	*new_node;
	t_env_list	*head;

	head = env_list;
	new_node = NULL;
	dup_env = NULL;
	while (head != NULL)
	{
		dup_key = ft_strdup(head->key);
		if (head->value)
			dup_value = ft_strdup(head->value);
		else
			dup_value = NULL;
		new_node = create_key_value(dup_key, dup_value);
		ms_free_string(dup_key);
		ms_free_string(dup_value);
		token_add_back_env(&dup_env, new_node);
		head = head->next;
	}
	return (dup_env);
}
