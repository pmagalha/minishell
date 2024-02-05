/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 16:07:55 by joao-ppe          #+#    #+#             */
/*   Updated: 2024/02/05 13:56:42 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_signs(char *str, char c)
{
	int	i;

	i = 0;
	while (str[i] == c)
		i++;
	printf("NUMBER OF SIGNS: %d\n", i);
	return (i);
}

static char	*set_sign_value(char *content, int signs, char *value) // PENSAR NOUTRO NOME PARA ESTA FUNCAO
{
	char	*new_content;
	char	*temp;

	new_content = NULL;
	temp = NULL;
	if (signs == 1)
		return (ft_strdup(value));
	if (signs == 2)
		return (ft_strjoin(ft_itoa(getpid()), content));
	else if (signs > 2)
	{
		if (signs % 2)
		{
			new_content = ft_strjoin(ft_itoa(getpid()), value);
			signs -= 1;
		}
		else
			new_content = ft_strjoin(ft_itoa(getpid()), content);
		temp = new_content;
		signs -= 2;
		while (signs > 0)
		{
			new_content = ft_strjoin(ft_itoa(getpid()), new_content);
			free (temp);
			if (signs > 2)
				temp = new_content;
			signs -= 2;
		}
		return (new_content);
	}
	return (content);
}

void	expander(t_lexer *lexer, t_env_list *env_list)
{
	t_lexer	*head;

	head = lexer;

	while (head != NULL)
	{
		printf("CONTENT: [%s]\n", head->content);
		if (head->content[0] == '$' && !head->content[1])
			return ;
		else if (head->content[0] == '$' && head->content[1] == '\"'
				&& head->content[ft_strlen(head->content)] == '\"')
		{
			head->content = trim_quotes(head->content);
			return ;
		}
		else if (head->content[0] == '$' && head->content[1]) // verificar o caso de ter digitos NUMERARIOS depois do $
			head->content = set_key_value(head->content, env_list); // funcao para trocar o conteudo da string content pelo valor correspondente do env_list
		head = head->next;
	}
}

char	*set_key_value(char *content, t_env_list *env_list)
{
	t_env_list	*head;
	char		*new_value;
	int			signs;

	head = env_list;
	new_value = NULL;
	signs = count_signs(content, '$');
	while (head != NULL)
	{
		if (!ft_strncmp(content + signs, head->key, ft_strlen(head->key)))
		{
			new_value = set_sign_value(content + signs, signs, head->value);
			free (content);
			return (new_value);
		}
		head = head->next;
	}
	if (signs > 1)
	{
		new_value = set_sign_value(content + signs, signs, content + signs);
		free (content);
		return (new_value);
	}	
	return (""); // PERGUNTAR AO DIOGO QUAL SERA O RETURN NO CASO DE FAZERMOS ECHO >> $BOAS visto que $BOAS NAO EXISTE
}
// IMPORTANTE : TRATAR DO CASO DE SER $1USER E DAS MERDAS DAS QUOTES