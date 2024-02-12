/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/15 16:11:33 by pmagalha          #+#    #+#             */
/*   Updated: 2024/02/08 14:26:36 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* char *get_key(char *env) estas funcoes serviam para retirar os valores da key, value e string, que foram substituidas por substrings
{
    int i;
    int len;
	char	*key;

	i = 0;
	len = 0;
    while (env[i] != '=')
    {
        len++;
        i++;
    }
    key = malloc(sizeof(char) * (len + 1));
	if (!key)
		return (NULL);
    i = 0;
    while (i < len)
    {
        key[i] = env[i];
        i++;
    }
    key[len] = '\0';
	//printf("ISTO EH UMA KEY: %s\n\n", key);
    return (key);
} */

/* char *get_value(char *env) estas funcoes serviam para retirar os valores da key, value e string, que foram substituidas por substrings
{
    int i;
    int len;
	char *value;

    i = 0;
    len = 0;
    while (env[i] != '=')
        i++;
    i++;
	len = ft_strlen(env + i);
    value = malloc(sizeof(char) * (len + 1));
	if (!value)
		return (NULL);
	ft_strlcpy(value, env + i, len + 1);
    //printf("ISTO EH UM VALUE: %s\n\n", value);
    return (value); // SE DER MERDA EH NESTA FUNCAO POR CAUSA DO LEN
} */

/* char	*get_full_string(char *env) estas funcoes serviam para retirar os valores da key, value e string, que foram substituidas por substrings
{
	int		i;
	int		len;
	char	*full_string;

	i = 0;
	len = ft_strlen(env);
	full_string = malloc(sizeof(char) * len + 1);
	if (!full_string)
		return (NULL);
	ft_strlcpy(full_string, env, len + 1);
	//printf("ISTO EH UMA FULL STRING: %s\n\n", full_string);
	return (full_string);
} */

//alterei eesta fill string para nao receber o line count 

// tudo que esta acima daqui provavelmente esta bem mas eh melhor checkar com prints
// o que esta para baixo nao esta bem, porque nao estamos a criar o node corretamente.

/* char	**duplicate_env(char **env) esta funcao ja nao eh necessaria porque agora passamos o env todo para uma lista que depois vamos manipular
{
	int	i;
	int	lines;
	char	**dup_env;
	
	i = 0;
	lines = count_lines(env);
	dup_env = (char **)malloc((lines + 1) * sizeof(char *));
	while (i < lines)
	{
		dup_env[i] = ft_strdup(env[i]);
		if (!dup_env)
		{
			printf("Error memory allocation in dup_env");
			free(dup_env);
			break ;
			// METER AQUI OU EM ALGUM OUTRO LADO PARA DAR FREE DA DUP ENV
		}
		i++;
	}
	dup_env[lines] = NULL;
	return (dup_env);
}
 */

/* 
int	lines_len(char **env, int line_count)
{
	int	i;
	int	char_count;

	i = 0;
	char_count = 0;
	line_count = count_lines(env);
	while (i < line_count)
	{
		char_count = ft_strlen(env[i]);
		i++;
	}
	return (char_count);
}
 */

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