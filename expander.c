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

static int	is_identifier(char c)
{
	return (c == '|' || c == '<' || c == '>' || c == '[' || c == ']'
		|| c == '\'' || c == '\"' || c == ' ' || c == ',' || c == '.'
		|| c == ':' || c == '/' || c == '{' || c == '}' || c == '+'
		|| c == '^' || c == '%' || c == '#' || c == '@' || c == '!'
		|| c == '~'	|| c == '=' || c == '-' || c == '?' || c == '&' || c == '*');
}

static char *expand_single_quotes(char *input)
{
	char	*new;

	new = NULL;
	new = copy_content(new, input + 1, '\'');
	return (new);
}

// echo -n hello $false $USER file | cat $$ $USER$PWD$2LANG $? file_2 | pwd | "$?" | '$LANG' $"test" |
static char *expand_double_quotes(char *input, t_env_list *env_list)
{
	char	*new = NULL;
	char	*key = NULL;
	char	*newinput;
	char	*tmp;

	////////printf("QUOTES INPUT: [%s]\n", input);
	newinput = ft_strndup(input, ft_strclen(input + 1, '"') + 1);
	key = NULL;
	tmp = newinput;
	while (*newinput)
	{
		if (*newinput == '$' && *(newinput + 1))
		{
			if (is_identifier(*(newinput + 1)) || *(newinput + 1) == '$')
			{
				key = ft_strndup(newinput, 1);
				newinput += 2;
			}
			else
			{
				key = ft_strndup(newinput, ft_strclen(newinput + 1, next_char_space(newinput + 1))); // nao mexer crl
				newinput += ft_strclen(newinput + 1, next_char_space(newinput + 1));
			}
			if (new)
				new = ft_strjoin(new, expander(key, env_list));
			else
				new = expander(key, env_list);
		}
		else if ((*newinput != '\"' && *newinput != '\'' && *newinput != '$'))
		{
			if (is_identifier(*input))
			{
				new = ms_safejoin(new, ft_substr(newinput, 0, 1));
				/* if (new)
					new = ft_strjoin(new, ft_substr(newinput, 0, 1));
				else
					new = ft_substr(newinput, 0, 1); */
			}
			newinput++;
			continue ;
			new = copy_content(new, newinput, next_char(newinput + 1));
			newinput += ft_strclen(newinput, next_char(newinput + 1)) - 1;
			if (*newinput == '"')
				newinput++;
		}
		else if (*newinput == '\'' && sign_exists(newinput + 1, '\''))
		{
			new = ms_safejoin(new, expand_single_quotes(newinput));
			newinput += ft_strclen(newinput + 1, '\'');
		}
		if (*newinput != '$')
			newinput++;
	}
	if (tmp)
		free(tmp);
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
char	*expander(char *input, t_env_list *env_list)
{
	char	*new;

	new = NULL;
	//////printf("RAW INPUT: [%s]\n", input);
	while (*input != '\0')
	{
		if ((*input != '$' && *input != '\'' && *input != '\"')
			|| *input == '>')
		{
			if (is_identifier(*input))
			{
				new = ms_safejoin(new, ft_substr(input, 0, 1));
				/* if (new)
					new = ft_strjoin(new, ft_substr(input, 0, 1));
				else
					new = ft_substr(input, 0, 1); */
				input++;
				continue ;
			}
			new = copy_content(new, input, next_char(input + 1));
			input += ft_strclen(input, next_char(input + 1)) - 1;
		}
		else if (*input == '\'' || *input == '\"')
		{
			new = ms_safejoin(new, expand_quotes(input, env_list));
			/* if (new)
				new = ft_strjoin(new, expand_quotes(input, env_list));
			else
				new = expand_quotes(input, env_list); */
			input += ft_strclen(input + 1, *input) + 1;
		}
		else if (*input == '$' && *(input + 1) == '?')
		{
			new = ms_safejoin(new, ft_itoa(g_code));
			/* if (new)
				new = ft_strjoin(new, ft_itoa(g_code));
			else
				new = ft_strdup(ft_itoa(g_code)); */
			input += 1;
		}
		else if (*input == '$' && *(input + 1) == '$')
		{
			new = ms_safejoin(new, ft_itoa(getpid()));
			input += 1;
		}
	 	else if (*input == '$' && *(input + 1))
		{
			new = get_key_value(new, input, env_list);
			input += ft_strclen(input + 1, next_char(input + 1));
		}
		input++;
	}
	return (new);
}
