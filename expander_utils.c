/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-ppe <joao-ppe@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 15:20:21 by joao-ppe          #+#    #+#             */
/*   Updated: 2024/02/28 15:20:21 by joao-ppe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	next_char(char *input)
{
	int		i;

	i = -1;
	while (input[++i])
	{
		if (!ft_isalpha(input[i]))
			return (input[i]);
	}
	return (0);
}

char	*copy_content(char *new_str, char *input)
{
	char	*new; // nova string para onde se copia tudo

	new = NULL;
	if (!new_str) // new_str eh a string antiga
	{
		new = ft_strndup(input, ft_strclen(input, '$') - 1);
		return (new);
	}
	else
	{
		//printf("////////// START COPY [%s] ///////// N BYTES [%ld]\n", input, ft_strclen(input, next_char(input)));
		new = ft_strjoin(new_str, ft_strndup(input, ft_strclen(input, next_char(input))));
		//printf("[%s] ////////// NEW 1\n", new);
		if (new_str)
			free (new_str);
	}
	return (new);
}

static char	*find_value(char *key, t_env_list *env_list)
{
	t_env_list	*head;

	head = env_list;
	
	//printf("[%s] ////////// KEY TO FIND\n", key);
	while (head != NULL)
	{
		if (!ft_strncmp(key, head->key, ft_strlen(head->key)))
			return (ft_strdup(head->value));
		head = head->next;
	}
	return (NULL);
}

char	*get_key_value(char *new_str, char *input, t_env_list *env_list)
{
	char	*key;
	char	*value;
	char	*new;

	new = NULL;
	key = ft_strndup(input + 1, ft_strclen(input + 1, ' ')); // isolar a key do INPUT, sem espacos e $ para ser mais facil comparar na lista do ENV
	value = find_value(key, env_list);
	//printf("[%s] ////////// VALUE AFTER SEARCH\n", value);
	if (new_str && value)
		new = ft_strjoin(new_str, value);
	else if (!new_str && value)
		new = ft_strdup(value);
	else if (!new_str && !value)
		new = ft_strdup("");
	if (key)
		free (key);
	//printf("============== ENTREEEEEEEEEEEEEEEEEEEEEEEEEEI ==============\n");
	if (new_str)
		free (new_str);
	if (value != NULL)
		free (value);
	return (new);
}

bool	sign_exists(char *str, char c) // pensar noutro nome desta funcao, meter algo mais bacano
{
	int	i;
	
	i = -1;
	while (str[++i])
	{
		if (str[i] == c)
			return (true);
	}
	return (false);
}

int	count_signs(char *str, char c)
{
	int	i;

	i = 0;
	while (str[i] == c)
		i++;
	return (i);
}
