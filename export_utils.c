/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-ppe <joao-ppe@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 13:55:11 by pmagalha          #+#    #+#             */
/*   Updated: 2024/04/04 14:21:39 by joao-ppe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sort_export(t_env_list *dup_env)
{
	int			i;
	t_env_list	*current;
	t_env_list	*head;

	i = 0;
	current = dup_env;
	head = dup_env;
	while (current && current->next)
	{
		if (ft_strncmp(current->key, current->next->key, i) > 0)
		{
			swap(current, current->next);
			current = head;
			i = 0;
		}
		else
		{
			current = current->next;
			i++;
		}
	}
}

void	print_export(t_env_list *current)
{
	printf("declare -x %s", current->key);
	if (current->value && *current->value)
		printf("=\"%s\"\n", current->value);
	else if (current->value)
		printf("=\"\"\n");
	else
		printf("\n");
}

void	add_on_env_list(t_env_list *env_list, char *key, char *value)
{
	t_env_list	*new_node;

	new_node = create_key_value(key, value);
	if (new_node)
		token_add_back_env(&env_list, new_node);
}

char	*extract_key(char *variable)
{
	char	*key;

	key = NULL;
	if (ft_strchr(variable, '='))
		key = ft_substr(variable, 0, ft_strchr(variable, '=') - variable);
	else
		key = ft_strdup(variable);
	return (key);
}

char	*extract_value(char *variable)
{
	char	*value;

	value = NULL;
	if (ft_strchr(variable, '='))
		value = ft_substr(ft_strchr(variable, '=') + 1, 0,
				ft_strlen(variable) - (ft_strchr(variable, '=')
					- variable + 1));
	return (value);
}
