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

char	*expand_quotes(char *input)
{
	char	*new_input;

	//printf("\033[32;1m=========== EXPANDER QUOTES DEV MOD ==========\033[0m\n");
	new_input = NULL;
	//printf("INPUT POS QUOTES    =================== [%s]\n", input);
	if (*input == '\"' || *input == '\'')
	{
		if (*input == '\"') {
	//		printf("////// FOUND DOUBLE QUOTES\n");
			new_input = copy_content(new_input, input + 1, '\"');
		}
		else if (*input == '\'') {
	//		printf("////// FOUND SINGLE QUOTES\n");
			new_input = copy_content(new_input, input + 1, '\'');
		}
	}
	//printf("AFTER QUOTES INP    =================== [%s]\n", new_input);
	return (new_input);
}

// 'dadawdwad'   $USER $$$PWD       "$USER"'"$USER"' boas?

char	*expander(char *input, t_env_list *env_list)
{
	char	*new;

	new = NULL;
	printf("RAW INPUT        =================== [%s]\n", input);
	while (*input != '\0')
	{
		if ((*input != '$' && *input != '\'' && *input != '\"') || *input == 32)
		{
			if (*input == 32)
			{
				new = ft_strjoin(new, " ");
				input++;
				continue ;
			}
			new = copy_content(new, input, next_char(input + 1));
			input += ft_strclen(input, next_char(input + 1)) - 1;
		}
		else if (*input == '\'' || *input == '\"')
		{
			if (*input == '\"')
			{
				input++;
				continue ;
			}
			if (new)
				new = ft_strjoin(new, expand_quotes(input));
			else
				new = expand_quotes(input);
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

// IMPORTANTE - EXPANDER
// CORRIGIR ESTE CASO: [$12PWD$USER$$LANG$LSCOLORS] - depois do $$ imprime tudo fodido
// [$12PWD$USER$$LANG$LSCOLORS$$$32LANG] / testar este caso