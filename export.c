/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-ppe <joao-ppe@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 18:16:11 by pmagalha          #+#    #+#             */
/*   Updated: 2024/05/08 17:57:22 by joao-ppe         ###   ########.fr       */
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
		{
			ms_free_string(string);
			return (1);
		}
	}
	ms_free_string(string);
	return (0);
}

void	export_error(char *content)
{
	ft_putstr_fd("minishell: export: `", STDERR_FILENO);
	if (content[0] == 32 || (content[0] > 9 && content[0] < 13))
		ft_putstr_fd(content, STDERR_FILENO);
	ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
}

// o problema aqui eh que o get_token so guarda ate ao igual, por causa do espaco a seguir,
// Por outras palavras: CMD1[export] CMD2[a=] CMD3[""] - O CMD3 nao existe porque esta vazio, entao nao da para prever o erro
int	check_export(t_prompt *prompt, t_parser *parser)
{
	t_lexer	*cmd;

	cmd = parser->command->next;
	while (cmd)
	{
		if (cmd->content[0] && cmd->content[0] != '='
			&& !check_export_arg(ft_strndup(cmd->content, 
				ft_strclen(cmd->content, '='))))
		{
			add_value(cmd->content, prompt);
			cmd = cmd->next;
		}
		else
		{
			export_error(cmd->content);
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
	else
	{
		ms_free_string(current->value);
		current->value = NULL;
	}
	ms_free_string(key);
	return (1);
}

int	add_value(char *variable, t_prompt *prompt)
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
			return (update_existing_value(key, value, current));
		current = current->next;
	}
	add_on_env_list(prompt->env_list, key, value);
	ms_free_string(value);
	ms_free_string(key);
	return (0);
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
