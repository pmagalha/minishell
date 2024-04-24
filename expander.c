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
    char    *new;

    new = NULL;
	if (ft_strlen(input) == 2)
		return (new);
	new = copy_content(new, input + 1, *input);
    return (new);
}

static char *expand_double_quotes(char *input, t_env_list *env_list)
{
    char    *new;
    char    *newinput;
    char    *tmp;
    char    *key;
	char	*tmp2;

    newinput = ft_strndup(input, ft_strclen(input + 1, '"') + 1);
	new = NULL;
    tmp = newinput;
	tmp2 = NULL;
    key = NULL;
    while (*newinput)
    {
		if (*newinput == '$') // fazer funcao para passar o &new e modificar diretamente na nova funcao, cuja funcao retorna o tamanho da key ?
		{
			if (*(newinput + 1) == '\'')
			{
				new = ms_safejoin(new, ft_strdup("$'"));
				newinput += 2;
				continue ;
			}
			else if (*(newinput + 1) == 32)
			{
				new = ms_safejoin(new, ft_strdup("$"));
				newinput++;
				continue ;
			}
			key = get_key(newinput);
			new = ms_safejoin(new, expander(key, env_list));
			newinput += ft_strlen(key);
			ms_free_string(key);
			continue ;
		}
		else if (*newinput != '\"' && *newinput != '\'' && *newinput != '$')
		{
			if (is_identifier(*newinput))
			{
				tmp2 = ft_substr(newinput, 0, 1);
				new = ms_safejoin(new, tmp2);
				newinput++;
				continue ;
			}
			new = copy_content(new, newinput, next_char(newinput + 1));
			newinput += ft_strclen(newinput, next_char(newinput + 1)) - 1;
			if (*newinput == '"')
				newinput++;
		}
		else if (*newinput == '\'' && sign_exists(newinput + 1, '\'', '\"'))
		{
			tmp2 = expand_single_quotes(newinput);
			new = ms_safejoin(new, tmp2);
			newinput += ft_strclen(newinput + 1, '\'') + 1;
		}
		else if (*newinput == '\'')
		{
			tmp2 = ft_substr(newinput, 0, 1);
			new = ms_safejoin(new, tmp2);
		}
		if (*newinput != '$')
			newinput++;
		}
		if (tmp)
			free(tmp);
		return (new);
}

char    *expand_quotes(char *input, t_env_list *env_list)
{
    char    *new_input;

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

char    *expander(char *input, t_env_list *env_list)
{
    char    *new;
	char	*tmp;

    new = NULL;
	tmp = NULL;
	if (!*(input))
		return (NULL);
    while (*input)
    {
        if (*input != '$' && *input != '\'' && *input != '\"')
        {
            if (is_identifier(*input))
            {
				tmp = ft_substr(input, 0, 1);
                new = ms_safejoin(new, tmp);
                input++;
                continue ;
            }
            new = copy_content(new, input, next_char(input + 1));
            input += ft_strclen(input, next_char(input + 1)) - 1;
        }
        else if (*input == '\'' || *input == '\"')
        {
			tmp = expand_quotes(input, env_list);
            new = ms_safejoin(new, tmp);
            input += ft_strclen(input + 1, *input) + 1;
        }
        else if (*input == '$' && *(input + 1) == '?')
        {
            new = ms_safejoin(new, ft_itoa(g_code));
            input++;
        }
        else if (*input == '$' && *(input + 1) == '$')
        {
            new = ms_safejoin(new, ft_itoa(getpid()));
            input++;
        }
        else if (*input == '$')
        {
            new = get_key_value(new, input, env_list);
            input += ft_strclen(input + 1, next_char(input + 1));
        }
        input++;
    }
    return (new);
}
