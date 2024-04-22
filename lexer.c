/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-ppe <joao-ppe@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 18:09:16 by pmagalha          #+#    #+#             */
/*   Updated: 2024/04/22 14:17:14 by joao-ppe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	get_token(char *input, t_prompt *prompt)
{
	char	*content;
	char	*new_content;
	int		len;
	t_type	type;
	
	new_content = NULL;
	len = 0;
 	if (!check_quotes(input))
			exit (1);
	while (*input)
	{
		while (*input == 32)
			input++;
		if (!*input)
			break ;
		content = get_token_content(prompt, input);
		len = ft_strlen(content);
		new_content = expander(content, prompt->env_list);
		input += len;
		if (new_content == NULL)
		{	
			free (content);
			continue ;
		}
		type = get_type(new_content);
		free (content);
		token_add_back(&(prompt->lexer), create_node(new_content, type));
        if (prompt->lexer->type == PIPE && !prompt->lexer->next) // this is the case for when there is only a single PIPE and nothing after it
		{
			ft_printf("minishell: syntax error near unexpected token `|'\n"); // o exit code vai ser 2
			exit(2); // fazer um msexit para quando houver um erro fazer os frees e dar exit do programa | 
            // lidar com erro para quando ha redirect e nada depois. O nome do erro eh "minishell: syntax error near unexpected token `newline' (mas se for tipo >>>> tem de dar ">>") o exit code disto vai ser 2"
            // quando a palavra a frente do redirect eh uma variavel env que tem de ser substituida mas nao existe. Nome "minishell: ambiguous redirect" e o exit code eh 1
		}
	}
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

char    *other_content(char *input)
{
    int i;
    bool in_quotes;
    char *res;
    int quote;

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
                || input[i] == '|') && !in_quotes) // it breaks and exists the loop in case it finds any operator and is not between quotes
            break ;
    }
    res = ft_calloc(i + 1, sizeof(char));
    ft_strlcpy(res, input, i + 1);
    return (res);
}

// NAO ESQUECER DE TRATAR TABS E OUTROS WHITESPACES