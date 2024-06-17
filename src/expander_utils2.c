/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-ppe <joao-ppe@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 16:57:49 by joao-ppe          #+#    #+#             */
/*   Updated: 2024/06/17 18:01:16 by joao-ppe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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

char	*get_key(char *input)
{
	char	*key;

	key = NULL;
	if ((is_identifier(*(input + 1)) || *(input + 1) == '$')
		&& (*(input + 1) != '\'' && *(input + 1) != '\"'))
		key = ft_strndup(input, 2);
	else if (ft_isalnum(*(input + 1)))
		key = ft_strndup(input, ft_strclen(input, next_char_space(input + 1)));
	else
		key = ft_strdup("$");
	return (key);
}

int	is_identifier(char c)
{
	return (c == '|' || c == '<' || c == '>' || c == '[' || c == ']'
		|| c == '\'' || c == '\"' || c == ' ' || c == ',' || c == '.'
		|| c == ':' || c == '/' || c == '\\' || c == '{' || c == '}'
		|| c == '^' || c == '%' || c == '#' || c == '@' || c == '!'
		|| c == '~' || c == '=' || c == '-' || c == '?' || c == '&'
		|| c == '*' || c == ';' || c == ')' || c == '(' || c == '+'
		|| c == '`');
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
