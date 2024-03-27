/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-ppe <joao-ppe@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 18:16:11 by pmagalha          #+#    #+#             */
/*   Updated: 2024/03/27 18:55:55 by joao-ppe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		new_node = create_key_value2(dup_key, dup_value);
		token_add_back_env(&dup_env, new_node);
		head = head->next;
	}
	return (dup_env);
}

int	check_export_arg(char *string)
{
	int	i;

	i = 0;
	if (!string)
		return (0);
	while (string[i])
	{
		if (isalpha(string[i]) || (string[i] == '=' && i != 0))
			i++;
		else
			return (1);
	}
	return (0);
}

void	check_export(t_prompt *prompt)
{
	t_lexer	*command;

	command = prompt->parser->command->next;
	while (command)
	{
		if (command->content[0] != '='
			&& !check_export_arg(ft_strndup(command->content,
					ft_strclen(command->content, '='))))
		{
			add_value(command->content, prompt);
			command = command->next;
		}
		else
		{
			printf("minishell: export: `%s': not a valid identifier\n",
				command->content);
			return ;
		}
	}
}

void	add_value(char *variable, t_prompt *prompt)
{
	char		*key;
	char		*value;
	t_env_list	*current;

	key = NULL;
	value = NULL;
	if (ft_strchr(variable, '='))
	{
		key = extract_key(variable);
		value = extract_value(variable);
	}
	else
		key = extract_key(variable);
	current = prompt->env_list;
	while (current)
	{
		if (!strcmp(current->key, key))
		{
			if (value)
				current->value = ft_strdup(value);
			return ;
		}
		current = current->next;
	}
	add_on_env_list(prompt->env_list, key, value);
}

void	ms_export(t_prompt *prompt)
{
	t_lexer		*command;
	t_env_list	*dup_env;
	t_env_list	*current;

	dup_env = create_dup(prompt->env_list);
	command = prompt->parser->command->next;
	if (!command)
	{
		current = dup_env;
		sort_export(dup_env);
		while (current)
		{
			print_export(current);
			current = current->next;
		}
	}
	check_export(prompt);
	/* if (dup_env)
		free_env_list(dup_env); THIS SHOULD PROBABLY BE DELETED (BREAKS EXPORT) */
}
