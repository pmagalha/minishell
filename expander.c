/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-ppe <joao-ppe@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 16:07:55 by joao-ppe          #+#    #+#             */
/*   Updated: 2024/01/30 16:41:06 by joao-ppe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	expander(t_lexer *lexer, t_env_list *env_list)
{
	t_lexer	*head;

	head = lexer;
	while (head != NULL)
	{
		if (head->content[0] == '$' && !head->content[1])
			return ;
		else if (head->content[0] == '$' && head->content[1] == '\"'
				&& head->content[ft_strlen(head->content)] == '\"')
		{
			head->content = trim_quotes(head->content);
			return ;
		}
		else if (head->content[0] == '$' && head->content[1]) // verificar o caso de ter digitos NUMERARIOS depois do $
		{
			head->content = set_key_value(head->content, env_list); // funcao para trocar o conteudo da string content pelo valor correspondente do env_list
//			printf("ACHEEEEEI\n");
		}	
		head = head->next;
	}
}

char	*set_key_value(char *content, t_env_list *env_list)
{
	t_env_list	*head;
	char		*new_value;

	head = env_list;
	new_value = NULL;
	while (*content == '$')
		content++;
	while (head != NULL)
	{
		if (!ft_strncmp(content, head->key, ft_strlen(content))) // $USER
		{
			new_value = ft_strdup(head->value);
			content--;
			free (content); // VERIFICAR ESTE FREE, ATENTION
			return (new_value);
		}
		head = head->next;
	}
	return (content);
}
