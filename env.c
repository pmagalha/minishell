/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmagalha <pmagalha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/15 16:11:33 by pmagalha          #+#    #+#             */
/*   Updated: 2024/03/25 13:58:35 by pmagalha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_lines(char **env)
{
	int	count;

	count = 0;
	while (*env)
	{
		count++;
		env++;
	}
	return (count);
}

t_env_list	*create_key_value(char *key, char *value, char *string)
{
	t_env_list	*new;

	new = (t_env_list *)malloc(sizeof(t_env_list));
	if (new == NULL)
		return (NULL);
	new->key = ft_strdup(key);
	new->value = ft_strdup(value);
	new->full_string = ft_strdup(string);
	new->next = NULL;
	return (new);
}

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

void	set_env(char **env, t_prompt *prompt)
{
	int			i;
	int			lines;
	char		*key;
	char		*value;
	char		*string;
	t_env_list	*new_node;

	i = 0;
	lines = count_lines(env);
	while (i < lines)
	{
		key = ft_substr(env[i], 0, ft_strchr(env[i], '=') - env[i]);
		value = ft_substr(env[i], ft_strchr(env[i], '=') - env[i] + 1, ft_strlen(env[i]));
		string = ft_strdup(env[i]);
		new_node = create_key_value(key, value, string);
		token_add_back_env(&(prompt->env_list), new_node);
		i++;
	}
    if (!get_env(prompt, "OLDPWD")) // este caso eh para quando um OLDPWD nao exite, por exemplo se abrirmos o terminal diretamente da janela do ms
	{
        key = "OLDPWD";
        value = get_env(prompt, "PWD");
        new_node = create_key_value(key, value, string);
        token_add_back_env(&(prompt->env_list), new_node);
	}
}

void	print_env_list(t_env_list *head)
{
	t_env_list *current = head;
	while (current != NULL)
	{
		printf("Key: %s | Value: %s | Full String: %s\n\n", current->key, current->value, current->full_string);
		current = current->next;
	}
}

void	print_env(char **env)
{
	int i;

	i = 0;
	while (env[i] != NULL)
	{
		printf("%s\n", env[i]);
		i++;
	}
}

//USAR A FUNCAO CREATE NODE PARA COLOCAR em env->key e env->value os valores antes e depois do =

//FAZER UM TRIM PARA TIRAR QUOTES

// conselho do Jonatha, usar o access no final do parser

// ter cuidado com o addhistory guardar comandos vazios