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
	//int		i;

	//i = 1;
	new = NULL;
	//printf("[%s] ////////// RAW INPUT\n", input);
	while (*input != '\0')
	{
		//printf("============== WORD [%d] - [%s] ==============\n", i, input);
		if (*input != '$')
		{
			new = copy_content(new, input); // copiar content até ao '$'
			//printf("////////// INPUT POS [%s] ///////// NEXT CHAR [%c] /////// SIZE TILL NEXT CHAR [%ld]\n", input, next_char(input), ft_strclen(input, next_char(input)));
			input += ft_strclen(input, next_char(input)); // avancar ate ao '$'
			//printf("[%s] ////////// INPUT AFTER COPY\n", input);
		}
	 	else if (*input == '$')
		{
			//printf("============== FOUND '$' FIRST ==============\n");
			new = get_key_value(new, input, env_list);
			//printf("[%s] ////////// NEW AFTER SWAP\n", new);
			input += ft_strclen(input + 1, next_char(input + 1));
			//printf("[%s] ////////// INPUT AFTER ADD\n", input);
		}
		//i++; // o i só serve para contar a palavra em que estou, em debug. depois é apagar
		input++;
	}
	return (new);
}
