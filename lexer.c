/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-ppe <joao-ppe@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 18:09:16 by pmagalha          #+#    #+#             */
/*   Updated: 2024/04/29 18:46:39 by joao-ppe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_code;

int	get_token(char *input, t_prompt *prompt)
{
	char	*content;
	char	*new_content;
	t_type	type;

	new_content = NULL;
	while (*input)
	{
		while (*input == 32 || (*input >= 9 && *input <= 13))
			input++;
		if (!*input)
			break ;
		content = get_token_content(prompt, input);
		type = get_type(content);
		if (content)
			new_content = expander(content, prompt->env_list, NULL);
		input += ft_strlen(content);
		if (check_content(content, new_content))
			continue ;
		free (content);
		token_add_back(&(prompt->lexer), create_node(new_content, type));
	}
	return (0);
}

char	*get_token_content(t_prompt *prompt, char *content)
{
	if (!content)
		return (NULL);
	if (*content == '>' || *content == '<' || *content == '|')
		return (get_operator(content));
	else if (*content == '\"' || *content == '\'')
		return (get_quoted_content(prompt, content));
	else
		return (other_content(content));
}

char	*get_operator(char *content)
{
	if (content[0] == '>' && content[1] == '>')
		return (ft_substr(content, 0, 2));
	else if (content[0] == '>' && content[1] != '>')
		return (ft_substr(content, 0, 1));
	else if (content[0] == '<' && content[1] == '<')
		return (ft_substr(content, 0, 2));
	else if (content[0] == '<' && content[1] != '<')
		return (ft_substr(content, 0, 1));
	else if (content[0] == '|')
		return (ft_substr(content, 0, 1));
	return (NULL);
}

t_type	get_type(char *content)
{
	if (content[0] == '>' && content[1] != '>')
		return (REDIR_OUT);
	else if (content[0] == '<' && content[1] != '<')
		return (REDIR_IN);
	else if (content[0] == '<' && content[1] == '<')
		return (HEREDOC);
	else if (content[0] == '>' && content[1] == '>')
		return (REDIR2_OUT);
	else if (content[0] == '|')
		return (PIPE);
	else
		return (OTHER);
}

// APAGAR FUNCAO? TA GLITCHED EEEWWW
char	*other_content(char *input)
{
	int		i;
	bool	in_quotes;
	char	*res;

	i = -1;
	in_quotes = false;
	if (!input)
		return (NULL);
	while (input[++i])
	{
		if ((input[i] == '\'' && input[i + 1] == '\'')
			|| (input[i] == '\"' && input[i + 1] == '\"'))
		{
			i += 2;
			continue ;
		}
		else if (input[i] == '\'' || input[i] == '\"')
			in_quotes = !in_quotes;
		if ((input[i] == ' ' || input[i] == '<' || input[i] == '>'
				|| input[i] == '|') && !in_quotes)
			break ;
	}
	res = ft_calloc(i + 1, sizeof(char));
	ft_strlcpy(res, input, i + 1);
	return (res);
}
