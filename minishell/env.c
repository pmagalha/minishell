/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmagalha <pmagalha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/15 16:11:33 by pmagalha          #+#    #+#             */
/*   Updated: 2024/01/17 18:47:17 by pmagalha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int count_lines(char **env)
{
    int count;

	count = 0;
    while (*env)
    {
        count++;
        env++;
    }
    return (count);
}

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

char	**duplicate_env(char **env)
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

// Aqui vamos fazer duas funcoes para sacarem o antes e o depois do = no env
// Key = esquerda do =, Vale = direita do igual

char *get_key(char *env)
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
	printf("ISTO EH UMA KEY: %s\n\n", key);
    return (key);
}

char *get_value(char *env)
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
    printf("ISTO EH UM VALUE: %s\n\n", value);
    return (value); // SE DER MERDA EH NESTA FUNCAO POR CAUSA DO LEN
}

char	*get_full_string(char *env)
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
	printf("ISTO EH UMA FULL STRING: %s\n\n", full_string);
	return (full_string);
}

//alterei eesta fill string para nao receber o line count 

// tudo que esta acima daqui provavelmente esta bem mas eh melhor checkar com prints
// o que esta para baixo nao esta bem, porque nao estamos a criar o node corretamente.

t_env_node	*create_key_value(char *key, char *value, char *string)
{
	t_env_node	*new;

	new = (t_env_node *)malloc(sizeof(t_env_node));
	if (new == NULL)
		return (NULL);
	new->key = ft_strdup(key);
	new->value = ft_strdup(value);
	new->full_string = ft_strdup(string);
	new->next = NULL;
	return (new);
}

void token_add_back_env(t_env_node **env_node, t_env_node *new)
{
    t_env_node *new_node;

    if (*env_node == NULL)
        *env_node = new;
    else
    {
        new_node = *env_node;
        while (new_node->next != NULL)
            new_node = new_node->next;
        new_node->next = new;
    }
}

void get_env(char **dup_env, t_env_node *env_node)
{
	int	i;
	int	lines;
    char *key;
    char *value;
    char *string;
	t_env_node *new_node;

	i = 0;
	lines = count_lines(dup_env);
	while (i < lines)
	{
		key = get_key(dup_env[i]);
		value = get_value(dup_env[i]);
		string = get_full_string(dup_env[i]);
		new_node = create_key_value(key, value, string);
		printf("Adding to env_node: Key: %s | Value: %s | Full String: %s\n\n", new_node->key, value, string);
    	token_add_back_env(&(env_node), new_node);
		i++;
	}
}

/* void print_env_node(t_env_node *node)
{
    printf("Key: %s | Value: %s | Full String: %s\n", node->key, node->value, node->full_string);
} */

/* void print_env_list(t_env_node *head)
{
    t_env_node *current = head;
    while (current != NULL)
    {
        print_env_node(current);
        current = current->next;
    }
}

void print_env(char **env)
{
    int i;
	
	i = 0;
    while (env[i] != NULL)
    {
        printf("%s\n", env[i]);
        i++;
    } */

//USAR A FUNCAO CREATE NODE PARA COLOCAR em env->key e env->value os valores antes e depois do =

//FAZER UM TRIM PARA TIRAR QUOTES

// conselho do Jonatha, usar o access no final do parser

// ter cuidado com o addhistory guardar comandos vazios