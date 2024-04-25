/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmagalha <pmagalha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 18:09:16 by pmagalha          #+#    #+#             */
/*   Updated: 2024/04/25 12:38:10 by pmagalha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_code;

void	pipe_error(t_prompt *prompt)
{
	ft_printf("minishell: syntax error near unexpected token `|'\n");
	free_data(prompt);
	exit(g_code);
}

int	check_content(char *content, char *new_content)
{
	if (new_content == NULL)
	{
		free (content);
		return (1);
	}
	return (0);
}

int	get_token(char *input, t_prompt *prompt)
{
	char	*content;
	char	*new_content;
	int		len;
	t_type	type;

	new_content = NULL;
	len = 0;
	while (*input)
	{
		while (*input == 32 || (*input >= 9 && *input <= 13))
			input++;
		if (!*input)
			break ;
		content = get_token_content(prompt, input);
		len = ft_strlen(content);
		type = get_type(content);
		new_content = expander(content, prompt->env_list);
		input += len;
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
	else if (*content == '"' || *content == '\'')
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

char	*other_content(char *input)
{
	int		i;
	bool	in_quotes;
	char	*res;

	i = 0;
	in_quotes = false;
	if (!input)
		return (NULL);
	while (input[i] && (!in_quotes || input[i] != input[i - 1]))
	{
		if (input[i] == '\'' || input[i] == '\"')
			in_quotes = !in_quotes;
		if ((input[i] == ' ' || input[i] == '<' || input[i] == '>'
				|| input[i] == '|') && !in_quotes)
			break ;
		i++;
	}
	res = ft_calloc(i + 1, sizeof(char));
	ft_strlcpy(res, input, i + 1);
	return (res);
}

/* char	*other_content(char *input)
{
	int		i;
	bool	in_quotes;
	char	*res;
	int		quote;
 !content[3]
	i = 0;
	quote = -1;
	in_quotes = false;
	if (!input)
		return (NULL);
	while (input[i++])
	{
		if (input[i] == '\'' || input[i] == '\"')
		{
			if (!in_quotes)
				quote = i;
			if (!in_quotes && input[i] == input[quote])
				in_quotes = true;
			else if (in_quotes && input[i] == input[quote])
				in_quotes = false;
		}
		if ((input[i] == ' ' || input[i] == '<' || input[i] == '>'
				|| input[i] == '|') && !in_quotes)
			break ;
	}
	res = ft_calloc(i + 1, sizeof(char));
	ft_strlcpy(res, input, i + 1);
	return (res);
} */
// Ter em conta o caso poop | echo lala	cenas. os processos estao
// a atropelar uns aos outros for some reason, provavelmente printfs