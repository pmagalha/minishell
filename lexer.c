/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmagalha <pmagalha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 18:09:16 by pmagalha          #+#    #+#             */
/*   Updated: 2024/05/08 16:26:27 by pmagalha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_code;

int	get_token(char *input, t_prompt *prompt)
{
	int		i;
	t_type	type;
	char	*content;
	char	*new_content;

	i = 0;
	new_content = NULL;
	while (*input)
	{
		while (*input == 32 || (*input >= 9 && *input <= 13))
			input++;
		if (!*input)
			break ;
		content = get_token_content(prompt, input);
		if (*input == '|' && !content)
			return (reset_data(prompt), 2);
		type = get_type(content);
		if (type == HEREDOC)
			i = 2;
		if (content && i == 0)
			new_content = expander(content, prompt->env_list, NULL);
		else if (content && i-- != 0)
			new_content = ft_strdup(content);
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
	char	*operator;
	if (!content)
		return (NULL);
	if (*content == '>' || *content == '<' || *content == '|')
	{
		operator = get_operator(content);
		if (operator)
			return (operator);
		else
			return (NULL);
	}
	else if (*content == '\"' || *content == '\'')
		return (get_quoted_content(prompt, content));
	else
		return (other_content(content));
}

int	check_pipe(char *content)
{
	int	i;

	i = 0;
	while (content[i + 1] != '|' && content[i + 1])
	{
		if (content[i] != 32 && (content[i] < 9 || content[i] > 13))
			return (1);
		i++;
	}
	return (0);
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
	else if (content[0] == '|' && !check_pipe(content))
	{
		printf("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\n");
		pipe_error();

	}
	else if (content[0] == '|' && check_pipe(content))
	{
		printf("BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB\n");
		return (ft_substr(content, 0, 1));
	}
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
