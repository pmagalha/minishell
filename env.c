/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-ppe <joao-ppe@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/15 16:11:33 by pmagalha          #+#    #+#             */
/*   Updated: 2024/04/10 17:42:00 by joao-ppe         ###   ########.fr       */
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

/* t_env_list	*create_key_value(char *key, char *value, char *string)
{
	t_env_list	*new;

	new = (t_env_list *)malloc(sizeof(t_env_list));
	if (new == NULL)
		return (NULL);
	new->key = key;
	new->value = value;
	new->full_string = string;
	new->next = NULL;
	return (new);
} */

t_env_list	*create_key_value(char *key, char *value)
{
	t_env_list	*new;

	new = (t_env_list *)malloc(sizeof(t_env_list));
	if (!new)
		return (NULL);
	new->key = ft_strdup(key);
	//if (!value)
	//	new->value = NULL;
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

void	set_env(char **env, t_prompt *prompt)
{
	int			i;
	int			lines;
	char		*key;
	char		*value;
	t_env_list	*new_node;

	i = 0;
	lines = count_lines(env);
	while (i < lines)
	{
		key = ft_substr(env[i], 0, ft_strchr(env[i], '=') - env[i]);
		value = ft_substr(env[i], ft_strchr(env[i], '=') - env[i] + 1, ft_strlen(env[i]));
		new_node = create_key_value(key, value);
		token_add_back_env(&(prompt->env_list), new_node);
		ms_free_string(key);
		ms_free_string(value);
		i++;
	}
    if (!get_env(prompt, "OLDPWD")) // este caso eh para quando um OLDPWD nao exite, por exemplo se abrirmos o terminal diretamente da janela do ms
	{
        key = ft_strdup("OLDPWD");
        value = get_env(prompt, "PWD");
        new_node = create_key_value(key, value);
        token_add_back_env(&(prompt->env_list), new_node);
		ms_free_string(key);
		ms_free_string(value);
	}
}

void	print_env_list(t_env_list *head)
{
	t_env_list *current = head;
	while (current != NULL)
	{
		printf("Key: %s | Value: %s \n", current->key, current->value);
		current = current->next;
	}
}
/* 
void	print_env(char **env)
{
	int i;

	i = 0;
	while (env[i] != NULL)
	{
		printf("%s\n", env[i]);
		i++;
	}
} */


//USAR A FUNCAO CREATE NODE PARA COLOCAR em env->key e env->value os valores antes e depois do =

//FAZER UM TRIM PARA TIRAR QUOTES

// conselho do Jonatha, usar o access no final do parser

// ter cuidado com o addhistory guardar comandos vazios