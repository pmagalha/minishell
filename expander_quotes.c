/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_quotes.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-ppe <joao-ppe@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 16:57:49 by joao-ppe          #+#    #+#             */
/*   Updated: 2024/04/01 14:53:34 by joao-ppe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char    *get_key(char *input)
{
    char    *key;

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
