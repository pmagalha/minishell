/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-ppe <joao-ppe@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 13:52:46 by pmagalha          #+#    #+#             */
/*   Updated: 2024/06/17 18:02:06 by joao-ppe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	delete_env(t_env_list *node)
{
	if (node == NULL)
		return (1);
	if (node->value)
	{
		free(node->key);
		free(node->value);
	}
	else
		free(node->key);
	free(node);
	return (0);
}

int	remove_value(char *variable, t_prompt *prompt)
{
	t_env_list	*prev;
	t_env_list	*current;

	prev = NULL;
	current = prompt->env_list;
	if (variable == NULL)
		return (1);
	if (!ft_strncmp(current->key, variable, ft_strlen(variable) + 1))
	{
		prompt->env_list = current->next;
		delete_env(current);
		return (1);
	}
	while (current != NULL)
	{
		if (!ft_strncmp(current->key, variable, ft_strlen(variable) + 1))
		{
			prev->next = current->next;
			delete_env(current);
			return (1);
		}
		prev = current;
		current = current->next;
	}
	return (0);
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

int	ms_unset(t_prompt *prompt, t_parser *parser)
{
	t_lexer	*command;

	command = parser->command->next;
	while (command != NULL)
	{
		remove_value(command->content, prompt);
		command = command->next;
	}
	return (0);
}
