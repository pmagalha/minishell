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

#include "../include/minishell.h"

char	next_char(char *str)
{
	int	i;

	i = -1;
	while (str[++i])
	{
		if ((!ft_isalnum(str[i]) && str[i] != 32) && str[i] != '_')
			return (str[i]);
	}
	return (0);
}

char	next_char_space(char *str)
{
	int	i;

	i = -1;
	while (str[++i])
	{
		if ((!ft_isalnum(str[i]) || str[i] == 32) && str[i] != '_')
			return (str[i]);
	}
	return (0);
}

char	*copy_content(char *new_str, char *input, char c)
{
	char	*new;
	char	*temp;

	new = NULL;
	temp = NULL;
	if (input)
		temp = ft_strndup(input, ft_strclen(input, c));
	new = ms_safejoin(new_str, temp);
	return (new);
}

char	*find_value(char *key, t_env_list *env_list)
{
	t_env_list	*head;

	head = env_list;
	while (head != NULL)
	{
		if (!ft_strncmp(key, head->key, ft_strlen(head->key) + 1))
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
	char	*temp;

	new = NULL;
	temp = NULL;
	if (*(input + 1) == '\0')
	{
		temp = ft_strdup("$");
		return (ms_safejoin(new_str, temp));
	}
	else if (*input == '$' && ft_isdigit(*(input + 1)))
		return (expand_digits(new_str, input));
	key = get_key(input + 1);
	input += ft_strclen(input, '$') - 1;
	value = find_value(key, env_list);
	new = ms_safejoin(new_str, value);
	if (key)
		free (key);
	return (new);
}
