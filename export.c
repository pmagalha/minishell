/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmagalha <pmagalha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 18:16:11 by pmagalha          #+#    #+#             */
/*   Updated: 2024/04/25 17:30:45 by pmagalha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	free(string);
	return (0);
}

int	check_export(t_prompt *prompt, t_parser *parser)
{
	t_lexer	*command;

	command = parser->command->next;
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
			ft_putstr_fd("minishell: export: `", STDERR_FILENO);
			ft_putstr_fd(command->content, STDERR_FILENO);
			ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
			return (1);
		}
	}
	return (0);
}

int	update_existing_value(char *key, char *value, t_env_list *current)
{
	if (value)
	{
		ms_free_string(current->value);
		current->value = ft_strdup(value);
		ms_free_string(value);
		ms_free_string(key);
		return (0);
	}
	ms_free_string(key);
	return (1);
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
			if (!update_existing_value(key, value, current))
				return ;
		current = current->next;
	}
	add_on_env_list(prompt->env_list, key, value);
	ms_free_string(value);
	ms_free_string(key);
}

int	ms_export(t_prompt *prompt, t_parser *parser)
{
	t_lexer		*command;
	t_env_list	*dup_env;
	t_env_list	*current;
	t_env_list	*head;

	dup_env = create_dup(prompt->env_list);
	command = parser->command->next;
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
	if (check_export(prompt, parser))
	{
		free_env_list(&head);
		return (1);
	}
	free_env_list(&head);
	return (0);
}
