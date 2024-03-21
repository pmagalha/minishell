/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmagalha <pmagalha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 18:16:11 by pmagalha          #+#    #+#             */
/*   Updated: 2024/03/21 16:23:46 by pmagalha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env_list	*create_key_value2(char *key, char *value)
{
	t_env_list	*new;

	new = (t_env_list *)malloc(sizeof(t_env_list));
	if (!new)
		return (NULL);
	new->key = ft_strdup(key);
	if (!value)
		new->value = NULL;
	else
		new->value = ft_strdup(value);
	new->next = NULL;
	return (new);
}

t_env_list	*create_dup(t_env_list *env_list)
{
	char		*dup_key;
	char		*dup_value;
	t_env_list	**dup_env;
	t_env_list	*new_node;
	t_env_list	*head;

	head = env_list;
	new_node = NULL;
	dup_env = (t_env_list **)malloc(sizeof(t_env_list *));
	while (head != NULL)
	{
		dup_key = ft_strdup(head->key);
		if (head->value)
			dup_value = ft_strdup(head->value);
		else
			dup_value = NULL;
		new_node = create_key_value2(dup_key, dup_value);
		token_add_back_env(dup_env, new_node);
		head = head->next;
	}
	return (*dup_env);
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

void	sort_export(t_env_list *dup_env)
{
	int			i;
	t_env_list	*current;
	t_env_list	*head;

	i = 0;
	current = dup_env;
	head = dup_env;
	while (current != NULL && current->next != NULL)
	{
		if (current->key[i] > current->next->key[i])
		{
			if (i > 0 && current->key[i - 1] > current->next->key[i - 1])
			{
				swap(current, current->next);
				current = head;
				i = 0;
			}
			else if (i == 0)
			{
				swap(current, current->next);
				current = head;
			}
			else
				current = current->next;
		}
		else
			current = current->next;
		if (current == NULL || current->key[i] == '\0')
		{
			i++;
			current = head;
		}
	}
}

int	check_export_arg(char *string)
{
	int	i;

	i = 0;
	while (string[i])
	{
		if (isalpha(string[i]) || (string[i] == '=' && i != 0))
			i++;
		else
			return (1);
	}
	return (0);
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
		while (current != NULL)
		{
			if (current->value && current->value[0])
			{
				// se houver key e value
				printf("11111111\n\n");
				printf("declare -x %s=\"%s\"\n", current->key, current->value);
				current = current->next;
			}
			// se nao houver value
			else
			{
				// se houver key mas o value for ""
				if (current->value && current->value[0] == '\0')
				{
					printf("2222222\n\n");
					printf("declare -x %s=\"\"\n", current->key);
					current = current->next;
				}
				// se houver key e o value nao existir
				else
				{
					printf("333333333\n\n");
					printf("declare -x %s\n", current->key);
					current = current->next;
				}
			}
		}
	}
	while (command)
	{
		if (command->content[0] != '=' && !check_export_arg(strndup(command->content,
					strchr(command->content, '=') - command->content)))
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
	t_env_list	*head;
	t_env_list	*new_node;
	t_env_list	*current;

	head = prompt->env_list;
	current = head;
	if (variable)
	{
		if (ft_strchr(variable, '='))
		{
			key = ft_substr(variable, 0, ft_strchr(variable, '=') - variable);
			value = ft_substr(ft_strchr(variable, '=') + 1, 0,
					ft_strlen(variable) - (ft_strchr(variable, '=')
						- variable + 1));
		}
		else
		{
			key = ft_strdup(variable);
			value = NULL;
		}
		if (value)
			new_node = create_key_value2(key, value);
		else
			new_node = create_key_value2(key, NULL);
		if (new_node)
		{
			while (current != NULL)
			{
				if (!strcmp(current->key, key))
				{
					if (value != NULL)
						current->value = ft_strdup(value);
					return ;
				}
				current = current->next;
			}
			token_add_back_env(&(prompt->env_list), new_node);
		}
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
		if (!strcmp(current->key, variable))
		{
			int i = 0;
			printf("%d, prev eh igual a NULL\n", i);
			prev->next = current->next;
			delete_env(current);
			return ;
		}
		prev = current;
		current = current->next;
	}
}

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
