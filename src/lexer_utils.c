/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-ppe <joao-ppe@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 19:28:15 by pmagalha          #+#    #+#             */
/*   Updated: 2024/06/17 18:01:40 by joao-ppe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_lexer	*create_node(char *content, t_type type)
{
	t_lexer	*new;

	new = (t_lexer *)malloc(sizeof(t_lexer));
	if (new == NULL)
		return (NULL);
	new->content = ft_strdup(content);
	new->type = type;
	new->next = NULL;
	new->prev = NULL;
	free(content);
	return (new);
}

void	token_add_back(t_lexer **token_lst, t_lexer *new)
{
	t_lexer	*new_node;

	if (*token_lst == NULL)
		*token_lst = new;
	else
	{
		new_node = *token_lst;
		while (new_node->next != NULL)
			new_node = new_node->next;
		new_node->next = new;
	}
}

bool	quotes_flag(t_prompt *prompt, char c)
{
	if (c == '"' && !prompt->quotes[0])
		prompt->quotes[1] = !prompt->quotes[1];
	else if (c == '\'' && !prompt->quotes[1])
		prompt->quotes[0] = !prompt->quotes[0];
	return (prompt->quotes[0] || prompt->quotes[1]);
}

int	pipe_error(void)
{
	ft_putstr_fd("minishell: syntax error", STDERR_FILENO);
	ft_putstr_fd(" near unexpected token `|'\n", STDERR_FILENO);
	g_code = 2;
	return (-1);
}

char	*get_quoted_content(t_prompt *prompt, char *input)
{
	char	*res;
	int		i;
	bool	in_quotes;

	i = 0;
	in_quotes = quotes_flag(prompt, input[i]);
	while (input[i++])
	{
		in_quotes = quotes_flag(prompt, input[i]);
		if ((input[i] == ' ' || input[i] == '<' || input[i] == '>'
				|| input[i] == '|') && !in_quotes)
			break ;
	}
	res = ft_calloc(i + 2, sizeof(char));
	if (!res)
		return (NULL);
	ft_strlcpy(res, input, i + 1);
	return (res);
}
