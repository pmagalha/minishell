/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_quotes.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-ppe <joao-ppe@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 16:57:49 by joao-ppe          #+#    #+#             */
/*   Updated: 2024/03/29 13:09:36 by joao-ppe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_key(char *input)
{
	char	*key;

	key = NULL;
	if ((is_identifier(*(input + 1)) || *(input + 1) == '$')
		&& (*(input + 1) != '\'' && *(input + 1) != '\"'))
		key = ft_strndup(input, 1);
	else if (ft_isalnum(*(input + 1)))
		key = ft_strndup(input, ft_strclen(input + 1, next_char_space(input + 1))); // nao mexer crl
	else
		key = ft_strdup("$");
	//printf("key: [%s]\n", key);
	return (key);
}
