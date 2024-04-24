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

int	is_identifier(char c)
{
	return (c == '|' || c == '<' || c == '>' || c == '[' || c == ']'
		|| c == '\'' || c == '\"' || c == ' ' || c == ',' || c == '.'
		|| c == ':' || c == '/' || c == '{' || c == '}' || c == '+'
		|| c == '^' || c == '%' || c == '#' || c == '@' || c == '!'
		|| c == '~' || c == '=' || c == '-' || c == '?' || c == '&'
		|| c == '*');
}

bool	sign_exists(char *str, char sign, char c)
{
	int	i;

	i = 0;
	if (!str && !str[1])
		return (false);
	while (str[i] && str[i] != c)
	{
		if (str[i] == sign)
			return (true);
		i++;
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
	key = ft_strndup(input + 1, ft_strclen(input + 1, next_char_space(input)));
	input += ft_strclen(input, '$') - 1;
	value = find_value(key, env_list);
	new = ms_safejoin(new_str, value);
	if (key)
		free (key);
	return (new);
}

char	*expand_digits(char *new_str, char *input)
{
	char	*temp;
	char	*new;

	new = NULL;
	temp = NULL;
	input = input + 2;
	if (input)
		temp = ft_strndup(input, ft_strclen(input, next_char(input)));
	new = ms_safejoin(new_str, temp);
	return (new);
}
