/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-ppe <joao-ppe@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 16:07:55 by joao-ppe          #+#    #+#             */
/*   Updated: 2024/02/14 13:14:05 by joao-ppe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_signs(char *str, char c)
{
	int	i;

	i = 0;
	while (str[i] == c)
		i++;
	return (i);
}

static char	*handle_digits(char *content, int signs)
{
	char	*new_content;
	char	*temp;

	new_content = NULL;
	temp = NULL;
	new_content = ft_strdup(content + signs);
	if (signs == 1 && !ft_isdigit(new_content[0]))
		return (new_content);
	else if (signs == 1 && ft_isdigit(new_content[0]))
	{
		temp = new_content;
		new_content = ft_strdup(new_content + 1);
		free (temp);
		return (new_content);
	}
	if (ft_isdigit(new_content[0]))
	{
		temp = new_content;
		new_content = ft_strdup(new_content + 1);
		free (temp);
	}
	while (signs > 0)
	{
		temp = new_content;
		new_content = ft_strjoin("$", new_content);
		free (temp);
		signs -= 1;
	}
	return (new_content);
}

static char	*set_sign_value(char *content, int signs, char *value) // FUNCAO PARA ATRIBUIR O VALOR DA VARIAVEL AO NODE DO LEXER E FAZ O REPLACE DOS CIFROES
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

static char	*handle_quotes(char *content)
{
	char	*new_content;
	char	*temp;

	temp = content;
	new_content = NULL;
	if (content[0] == '$')
		new_content = ft_strdup(temp + 1);
	else if (content[0] == '\"' || content[0] == '\'')
		new_content = trim_quotes(content);
	free (temp);
	printf("/////// NEW CONTENT: [%s]\n", new_content);
	return (new_content);
}

void	expander(t_lexer *lexer, t_env_list *env_list)
{
	t_lexer	*head;
	char	quote;

	head = lexer;

	while (head != NULL)
	{
		if (head->content[0] == '$' && !head->content[1])
			return ;
		else if ((head->content[0] == '$' && head->content[1] == '\"')
				|| (head->content[0] == '\"' || head->content[0] == '\''))
		{
			quote = head->content[0];
			head->content = handle_quotes(head->content);
			if (quote == '\'')
				return ;
		}
		if (head->content[0] == '$' && head->content[1])
			head->content = set_key_value(head->content, env_list); // funcao para trocar o conteudo da string content pelo valor correspondente do env_list
		head = head->next;
	}
}

char	*set_key_value(char *content, t_env_list *env_list)
{
	t_env_list	*head;
	char		*new_value;
	char		*temp;
	int			signs;

	head = env_list;
	new_value = NULL;
	temp = NULL;
	signs = count_signs(content, '$');
	if (ft_isdigit(content[signs]))
	{
		temp = content;
		content = handle_digits(content, signs);
		free (temp);
		if (signs == 1)
			return (content);
	}
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