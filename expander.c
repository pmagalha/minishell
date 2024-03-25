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

/* static char *expand_single_quotes(char *input)
{
	char	*new;

	new = copy_content(NULL, input, '\'');
	printf("NEW AFTER SINGLE QUOTES [%s]\n", new);
	return (new);
} */

static char *expand_double_quotes(char *input, t_env_list *env_list)
{
	char	*new;
	char	*temp;

	new = NULL;
	temp = NULL;
	while (*input != '\0')
	{
		printf("///////// DOUBLE QUOTES INPUT: [%s]\n\n", input);
		if (*input == '$')
		{
			temp = new;
			new = get_key_value(new, input, env_list);
			input += ft_strclen(input, next_char(input + 1));
			if (temp)
			free (temp);

		}
		else if (*input == '\'')
		{
			temp = new;
			new = ft_strjoin(new, copy_content(NULL, input + 1, '\''));
			input += ft_strclen(input, next_char(input + 1));
			if (temp)
				free (temp);
		}
		else if (*input == 32)
		{
			temp = new;
			new = ft_strjoin(new, " ");
			if (temp)
				free (temp);
		}
		else
		{
			new = ft_strjoin(new, copy_content(new, input, next_char(input + 1)));
			input += ft_strclen(input, next_char(input + 1));
		}
		printf("///////// DOUBLE QUOTES NEW: [%s]\n\n", new);
		input++;
		usleep(999999);
	}
	return (new);
}

char	*expand_quoted_content(char *input, char quotes, t_env_list *env_list)
{
	char	*new;
	char	*dup_input;
	//bool	*squote;
	//bool	*dquote;

	new = NULL;
	(void)quotes;
	dup_input = ft_strndup(input, ft_strclen(input, quotes));
	printf("DUP INPUT: [%s]\n\n", dup_input);
	if (quotes == '\"')
		new = expand_double_quotes(input, env_list);
	if (quotes == '\'')
		new = copy_content(NULL, input, '\'');
	if (dup_input)
		free (dup_input);
	return (new);
}

char	*expand_quotes(char *input, t_env_list *env_list)
{
	char	*new_input;

	printf("\033[32;1m=========== EXPANDER QUOTES DEV MOD ==========\033[0m\n");
	new_input = NULL;
	printf("INPUT POS QUOTES    =================== [%s]\n", input);
	if (*input == '\"' || *input == '\'')
	{
		if (*input == '\"')
			new_input = expand_quoted_content(input + 1,  '\"', env_list);
		else if (*input == '\'')
			new_input = expand_quoted_content(input + 1, '\'', env_list);
	}
	printf("AFTER QUOTES INP    =================== [%s]\n", new_input);
	return (new_input);
}

// 'dadawdwad'   $USER $$$PWD       "$USER"'"$USER"' boas?
// verificar o caso de não haver '$' no input, as quotes não são tratadas direito. ver se é preciso passar tudo por aqui na mesma, mesmo que não tenha '$'
char	*expander(char *input, t_env_list *env_list)
{
	char	*new;

	new = NULL;
	printf("RAW INPUT        =================== [%s]\n", input);
	while (*input != '\0')
	{
		printf("INPUT POS        =================== [%s]\n", input);
		if (*input != '$' && *input != '\'' && *input != '\"')
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
		printf("NEW              =================== [%s]\n", new);
		printf("\033[32;1m===========================================//========================================================\033[0m\n");
		usleep(999999);//para debug apenas, apagar depois
		input++;
	}
	return (new);
}

// IMPORTANTE - EXPANDER
// CORRIGIR ESTE CASO: [$12PWD$USER$$LANG$LSCOLORS] - depois do $$ imprime tudo fodido
// [$12PWD$USER$$LANG$LSCOLORS$$$32LANG] / testar este caso