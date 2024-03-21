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
	char	*temp;

	printf("\033[32;1m=========== EXPANDER QUOTES DEV MOD ==========\033[0m\n");
	new_input = NULL;
	temp = input;
	printf("TEMP: [%s]\n", temp);
	while (*input != 0)
	{
		printf("EXP QUOTES INP    =================== [%s]\n", input);
		if (*input == '\"' || *input == '\'')
		{
			if (*input == '\"') {
				printf("////// FOUND DOUBLE QUOTES\n");
				new_input = copy_content(new_input, input, *input);
			}
			else if (*input == '\'') {
				printf("////// FOUND SINGLE QUOTES\n");
				new_input = copy_content(new_input, input, *input);
			}
			input += ft_strclen(input + 1, *input) + 1;
		}
		else
		{
			new_input = copy_content(new_input, input, next_char(input));
			input += ft_strclen(input + 1, next_char(input)) + 1;
		}
		sleep(1);
		input++;
	}
	if (temp)
		free (temp);
	printf("AFTER QUOTES INP    =================== [%s]\n", input);
	return (new_input);
}

char	*expander(char *input, t_env_list *env_list)
{
	char	*new;

	new = NULL;
	printf("RAW INPUT        =================== [%s]\n", input);
	if (sign_exists(input, '\"') || sign_exists(input, '\''))
		input = expand_quotes(input);
	printf("INPUT AFTER QUO  =================== [%s]\n", input);
	while (*input != '\0')
	{
		printf("INPUT POS        =================== [%s]\n", input);
		if (*input != '$' || *input == 32 || (*input == '\'' && *(input + 1) == '$'))
		{
			new = copy_content(new, input, '$');
			input += ft_strclen(input, '$') - 1;
			//printf("INPUT AFTER COPY =================== [%s]\n", input);
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
		if (!*(input + 1) || !*input) // para evitar copiar merdas desnecessárias no fim, não sei o porquê deste bug // verificar se tirando esta condicao o codigo funciona direitinho
			break ;
		input++;
	}
	return (new);
}

// IMPORTANTE - EXPANDER
// CORRIGIR ESTE CASO: [$12PWD$USER$$LANG$LSCOLORS] - depois do $$ imprime tudo fodido
// [$12PWD$USER$$LANG$LSCOLORS$$$32LANG] / testar este caso