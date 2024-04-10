/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-ppe <joao-ppe@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 18:16:11 by pmagalha          #+#    #+#             */
/*   Updated: 2024/04/10 18:13:27 by joao-ppe         ###   ########.fr       */
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
		new_node = create_key_value(dup_key, dup_value);
		ms_free_string(dup_key);
		ms_free_string(dup_value);
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
		{
			//ms_free_array(string);
			return (1);
		}
	}
	free(string);
	return (0);
}

int	check_export(t_prompt *prompt)
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
			return (1);
		}
	}
	return (0);
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
		if (!ft_strncmp(current->key, key, ft_strlen(key) + 1))
		{
			if (value)
			{
				current->value = ft_strdup(value);
				ms_free_string(value);
			}
			ms_free_string(key);
			return ;
		}
		current = current->next;
	}
	add_on_env_list(prompt->env_list, key, value);
	ms_free_string(value);
	ms_free_string(key);
}

int	ms_export(t_prompt *prompt)
{
	t_lexer		*command;
	t_env_list	*dup_env;
	t_env_list	*current;
	t_env_list	*head;

	dup_env = create_dup(prompt->env_list);
	command = prompt->parser->command->next;
	head = dup_env;
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
	if (check_export(prompt))
	{
		free_env_list(&head);
		return (1);
	}
	free_env_list(&head);
	return (0);
}
