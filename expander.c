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

char	*expander(char *input, t_env_list *env_list)
{
	char	*new;

	new = NULL;
	//printf("RAW INPUT =================== [%s]\n", input);
	while (*input != '\0')
	{
		if (*input != '$' || (*input == '\'' && *(input + 1) == '$'))
		{
			new = copy_content(new, input);
			input += ft_strclen(input, next_char(input));
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
			//printf("OUTPUT     =================== [%s]\n", new);
			//printf("INPUT LOOP =================== [%s]\n", input);
		}
		input++;
	}
	return (new);
}

// IMPORTANTE - EXPANDER
// CORRIGIR ESTE CASO: [$12PWD$USER$$LANG$LSCOLORS] - depois do $$ imprime tudo fodido