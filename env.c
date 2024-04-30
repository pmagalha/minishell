/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmagalha <pmagalha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/15 16:11:33 by pmagalha          #+#    #+#             */
/*   Updated: 2024/04/30 15:41:30 by pmagalha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env_list	*create_key_value(char *key, char *value)
{
	t_env_list	*new;

	new = (t_env_list *)malloc(sizeof(t_env_list));
	if (!new)
		return (NULL);
	new->key = ft_strdup(key);
	if (value)
		new->value = ft_strdup(value);
	else
		new->value = NULL;
	new->next = NULL;
	return (new);
}

void	token_add_back_env(t_env_list **env_list, t_env_list *new)
{
	t_env_list	*new_node;

	if (*env_list == NULL)
		*env_list = new;
	else
	{
		new_node = *env_list;
		while (new_node->next != NULL)
			new_node = new_node->next;
		new_node->next = new;
	}
}

void	set_env_from_strings(char **env, t_prompt *prompt)
{
	int			i;
	int			lines;
	char		*key;
	char		*value;
	t_env_list	*new_node;

	i = 0;
	lines = count_lines(env);
	key = NULL;
	value = NULL;
	new_node = NULL;
	while (i < lines)
	{
		key = ft_substr(env[i], 0, ft_strchr(env[i], '=') - env[i]);
		if (!ft_strncmp(key, "SHLVL", ft_strlen(key) + 1))
		{
			value = ft_itoa(0);
			new_node = create_key_value(key, value);
			token_add_back_env(&(prompt->env_list), new_node);
			ms_free_string(key);
			ms_free_string(value);
			i++;
			continue;
		}
		value = ft_substr(env[i], ft_strchr(env[i], '=')
				- env[i] + 1, ft_strlen(env[i]));
		new_node = create_key_value(key, value);
		token_add_back_env(&(prompt->env_list), new_node);
		ms_free_string(key);
		ms_free_string(value);
		i++;
	}
}

void	update_shlvl(t_prompt *prompt)
{
	t_env_list *head;
	int			shlvl;
	char		*shlvl_value;

	shlvl_value = get_env(prompt, "SHLVL");
	shlvl = ft_atoi(shlvl_value);
	//ms_free_string(shlvl_value);
	head = prompt->env_list;
	printf("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\n");
	while (head)
	{
		if (!ft_strncmp(head->key, "SHLVL", ft_strlen(head->key) + 1))
		{
			ms_free_string(head->value);
			head->value = ft_itoa(shlvl + 1);
		}
		head = head->next;
	}
}

void	set_default_env(t_prompt *prompt)
{
	char		*key;
	char		*value;
	t_env_list	*new_node;
	t_env_list	*head;

	key = NULL;
	value = NULL;
	new_node = NULL;
	head = prompt->env_list;

	update_shlvl(prompt);
	if (!get_env(prompt, "OLDPWD"))
	{
		key = ft_strdup("OLDPWD");
		value = get_env(prompt, "PWD");
		new_node = create_key_value(key, value);
		token_add_back_env(&(prompt->env_list), new_node);
		ms_free_string(key);
		ms_free_string(value);
	}
}

void	set_env(char **env, t_prompt *prompt)
{
	set_env_from_strings(env, prompt);
	set_default_env(prompt);
}
