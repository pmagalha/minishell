/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-ppe <joao-ppe@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 16:07:55 by joao-ppe          #+#    #+#             */
/*   Updated: 2024/02/28 18:31:35 by joao-ppe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*expand_single_quotes(char *input)
{
	char	*new;

	new = NULL;
	if (ft_strlen(input) == 2)
		return (new);
	new = copy_content(new, input + 1, *input);
	return (new);
}

int	expand_quoted_sign(char *newinput, char **new, t_env_list *env_list)
{
	char	*key;
	int		len;

	len = 0;
	key = NULL;
	if (*(newinput + 1) == '\'')
	{
		*new = ms_safejoin(*new, ft_strdup("$'"));
		return (2);
	}
	else if (*(newinput + 1) == 32)
	{
		*new = ms_safejoin(*new, ft_strdup("$"));
		return (1);
	}
	key = get_key(newinput);
	*new = ms_safejoin(*new, expander(key, env_list, NULL));
	len = ft_strlen(key);
	ms_free_string(key);
	return (len);
}

static char	*expand_double_quotes(char *newinput, t_env_list *env_list)
{
	char	*new;
	char	*tmp;

	new = NULL;
	tmp = newinput;
	while (*newinput)
	{
		if (*newinput == '$')
		{
			newinput += expand_quoted_sign(newinput, &new, env_list);
			continue ;
		}
		else if (*newinput != '\"' && *newinput != '\'' && *newinput != '$')
		{
			if (check_quoted_content(&newinput, &new))
				continue ;
			if (*newinput == '\"')
				newinput++;
		}
		else if (*newinput == '\'')
			expand_single_quoted_content(&newinput, &new);
		if (*newinput != '$')
			newinput++;
	}
	return (ms_free_string(tmp), new);
}

char	*expand_quotes(char *input, t_env_list *env_list)
{
	char	*new_input;
	char	*temp;

	new_input = NULL;
	temp = NULL;
	if (*input == '\"' || *input == '\'')
	{
		if (*input == '\"')
		{
			temp = ft_strndup(input, ft_strclen(input + 1, '"') + 1);
			new_input = expand_double_quotes(temp, env_list);
		}
		else if (*input == '\'')
			new_input = expand_single_quotes(input);
	}
	return (new_input);
}

char	*expander(char *input, t_env_list *env_list, char *new)
{
	while (*input)
	{
		if (*input != '$' && *input != '\'' && *input != '\"')
		{
			if (is_identifier(*input))
			{
				new = ms_safejoin(new, ft_substr(input, 0, 1));
				input++;
				continue ;
			}
			new = copy_content(new, input, next_char(input + 1));
			input += ft_strclen(input, next_char(input + 1)) - 1;
		}
		else if (*input == '$')
			expand_signs(&input, &new, env_list);
		else if (*input == '\'' || *input == '\"')
		{
			new = ms_safejoin(new, expand_quotes(input, env_list));
			input += ft_strclen(input + 1, *input) + 1;
		}
		input++;
	}
	return (new);
}
