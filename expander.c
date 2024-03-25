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

#include "minishell.h"

static char *expand_single_quotes(char *input)
{
	char	*new;

	new = NULL;
	printf(" SINGLE QUOTE INPUT: [%s]\n", input);
	new = copy_content(new, input + 1, '\'');
	return (new);
}

static char *expand_double_quotes(char *input, t_env_list *env_list) // echo "Boas $USER 'tudo bem' '$PWD'"
{
	char	*new;

	new = NULL;
	(void)env_list;
	while (*input)
	{
		printf("INPUT POS: [%s]\n", input);
		if (*input != '$' && *input != '\'' && *input != '\"')
		{
			new = copy_content(new, input, next_char(input + 1));
			input += ft_strclen(input, next_char(input + 1)) - 1;
			printf("====== NEXT CHAR: [%c]\n", *input);
		}
		else if (*input == '$')
		{
			if (new)
				;
			else
				new = get_key_value(new, input, env_list);
		}
		input++;
	}
	return (new);
}

char	*expand_quotes(char *input, t_env_list *env_list)
{
	char	*new_input;

	new_input = NULL;
	if (*input == '\"' || *input == '\'')
	{
		if (*input == '\"')
			new_input = expand_double_quotes(input, env_list);
		else if (*input == '\'')
			new_input = expand_single_quotes(input);
	}
	return (new_input);
}

// 'dadawdwad'   $USER $$$PWD       "$USER"'"$USER"' boas?
// verificar o caso de não haver '$' no input, as quotes não são tratadas direito. ver se é preciso passar tudo por aqui na mesma, mesmo que não tenha '$'
char	*expander(char *input, t_env_list *env_list)
{
	char	*new;

	new = NULL;
	printf("RAW INPUT: [%s]\n", input);
	while (*input != '\0')
	{
		if (*input != '$' && *input != '\'' && *input != '\"')
		{
			new = copy_content(new, input, next_char(input + 1));
			input += ft_strclen(input, next_char(input + 1)) - 1;
		}
		else if (*input == '\'' || *input == '\"')
		{
			if (new)
				new = ft_strjoin(new, expand_quotes(input, env_list));
			else
				new = expand_quotes(input, env_list);
			input += ft_strclen(input + 1, *input) + 1;
		}
		else if (*input == '$' && *(input + 1) == '?')
		{
			if (new)
				new = ft_strjoin(new, ft_itoa(g_code));
			else
				new = ft_strdup(ft_itoa(g_code));
			input += 1;
		}
		else if (*input == '$' && *(input + 1) == '$')
		{
			if (new)
				new = ft_strjoin(new, ft_itoa(getpid()));
			else
				new = ft_strdup(ft_itoa(getpid()));
			input += 1;
		}
	 	else if (*input == '$' && *(input + 1) != '$')
		{
			new = get_key_value(new, input, env_list);
			input += ft_strclen(input + 1, next_char(input + 1));
		}
		input++;
	}
	return (new);
}
