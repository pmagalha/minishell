/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-ppe <joao-ppe@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 18:41:25 by joao-ppe          #+#    #+#             */
/*   Updated: 2024/06/17 18:01:18 by joao-ppe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	expand_signs(char **input, char **new, t_env_list *env_list)
{
	if (**(input) == '$' && (*(*input + 1)) == '?')
	{
		*new = ms_safejoin(*new, ft_itoa(g_code));
		*input += 1;
	}
	else if (**input == '$' && (*(*input + 1)) == '$')
	{
		*new = ms_safejoin(*new, ft_itoa(getpid()));
		*input += 1;
	}
	else if (**input == '$')
	{
		*new = get_key_value(*new, *input, env_list);
		*input += ft_strclen(*input + 1, next_char(*input + 1));
	}
}

int	expand_single_quoted_content(char **newinput, char **new)
{
	if (**newinput == '\'' && sign_exists(*newinput + 1, '\'', '\"'))
	{
		*new = ms_safejoin(*new, expand_single_quotes(*newinput));
		*newinput += ft_strclen(*newinput + 1, '\'') + 1;
	}
	else if (**newinput == '\'')
		*new = ms_safejoin(*new, ft_substr(*newinput, 0, 1));
	return (0);
}

int	check_quoted_content(char **newinput, char **new)
{
	if (!is_identifier(**(newinput)))
	{
		*newinput += copy_quoted_content(*newinput, new);
		return (0);
	}
	else
	{
		*newinput += copy_quoted_content(*newinput, new);
		return (1);
	}
	return (0);
}

int	copy_quoted_content(char *newinput, char **new)
{
	if (is_identifier(*newinput))
	{
		*new = ms_safejoin(*new, ft_substr(newinput, 0, 1));
		return (1);
	}
	*new = copy_content(*new, newinput, next_char(newinput + 1));
	return (ft_strclen(newinput, next_char(newinput + 1)) - 1);
}

char	*create_content(int i, t_env_list *env_list, char *content)
{
	char	*new_content;

	new_content = NULL;
	if (content && i == 0)
	{
		new_content = expander(content, env_list, NULL);
		return (new_content);
	}
	else if (content && i-- != 0)
	{
		new_content = ft_strdup(content);
		return (new_content);
	}
	return (0);
}
