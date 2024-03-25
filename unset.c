/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmagalha <pmagalha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 13:52:46 by pmagalha          #+#    #+#             */
/*   Updated: 2024/03/25 14:02:35 by pmagalha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ms_unset(t_prompt *prompt)
{
	t_lexer	*command;

	command = prompt->parser->command->next;
	while (command != NULL)
	{
		remove_value(command->content, prompt);
		command = command->next;
	}
}

void	delete_env(t_env_list *node)
{
	if (node == NULL)
		return ;
	if (node->value)
	{
		free(node->key);
		free(node->value);
	}
	else
		free(node->key);
	free(node);
}

void	remove_value(char *variable, t_prompt *prompt)
{
	t_env_list	*prev;
	t_env_list	*current;

	prev = NULL;
	current = prompt->env_list;
	if (variable == NULL)
		return ;
	while (current != NULL)
	{
		if (!ft_strncmp(current->key, variable, ft_strlen(variable)))
		{
			prev->next = current->next;
			delete_env(current);
			return ;
		}
		prev = current;
		current = current->next;
	}
}

void	swap(t_env_list *node1, t_env_list *node2)
{
	char	*temp_key;
	char	*temp_value;

	temp_key = node1->key;
	temp_value = node1->value;
	node1->key = node2->key;
	node1->value = node2->value;
	node2->key = temp_key;
	node2->value = temp_value;
}
