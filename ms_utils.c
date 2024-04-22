/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-ppe <joao-ppe@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 14:12:53 by joao-ppe          #+#    #+#             */
/*   Updated: 2024/04/22 14:16:24 by joao-ppe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *ms_safejoin(char *str1, char *str2)
{
    char *new;

    new = NULL;
    if (!str1 && !str2)
        return NULL;
    else if (!str1 && str2)
        new = ft_strdup(str2);
    else if (str1 && !str2)
		new = ft_strdup(str1);
    else
		new = ft_strjoin(str1, str2);
 	ms_free_string(str1);
	ms_free_string(str2);
    return (new);
}

void	reset_data(t_prompt *prompt)
{
	if (!prompt)
		return ;
	if (prompt->lexer)
		free_lexer_list(&prompt->lexer);
	if (prompt->parser)
		free_parser_list(&prompt->parser);
	if (prompt->pid)
		free(prompt->pid);
	prompt->pid = NULL;
	prompt->lexer = NULL;
	prompt->parser = NULL;
	prompt->reset = true;
}
