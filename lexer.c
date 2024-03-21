/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-ppe <joao-ppe@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 18:09:16 by pmagalha          #+#    #+#             */
/*   Updated: 2024/03/16 15:48:50 by joao-ppe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	get_token(char *input, t_prompt *prompt)
{
	char	*content;
	char	*temp;
	t_type	type;
	
	temp = NULL;
	printf("\033[32;1m=========== EXPANDER DEV MOD ==========\033[0m\n");
	if (!check_quotes(input))
			exit (1);
  	if (sign_exists(input, '$'))
	{
		temp = input;
		input = expander(input, prompt->env_list);
		free (temp);
	}
	//printf("[%s] ////////// INPUT AFTER EXPANDER\n", input);

	//ESTE TRIM QUOTES TEM DE SER ATIVADO QUANDO O EXPANDER ESTIVER BOM
	//input = trim_quotes(input);
	while (*input)
	{
		while (*input == 32)
			input++;
		if (!*input)
			break ;
		//CHECKING QUOTES HERE
		
		content = get_token_content(input);
		
		type = get_type(content);
		input += ft_strlen(content);
		token_add_back(&(prompt->lexer), create_node(content, type));
        if (prompt->lexer->type == PIPE && !prompt->lexer->next) // this is the case for when there is only a single PIPE and nothing after it
		{
			ft_printf("minishell: syntax error near unexpected token `|'\n"); // o exit code vai ser 2
			exit(2); // fazer um msexit para quando houver um erro fazer os frees e dar exit do programa | 
            // lidar com erro para quando ha redirect e nada depois. O nome do erro eh "minishell: syntax error near unexpected token `newline' (mas se for tipo >>>> tem de dar ">>") o exit code disto vai ser 2"
            // quando a palavra a frente do redirect eh uma variavel env que tem de ser substituida mas nao existe. Nome "minishell: ambiguous redirect" e o exit code eh 1
		}
	}
}

// Content: [$'HOME'] | Type: [OTHER] o lexer tem de dizer que se houver um $, tem de mandar com as quotes

char	*get_token_content(char *content)
{
	if (*content == '>' || *content == '<' || *content == '|')
		return (get_operator(content));
	else if (*content == '"' || *content == '\'')
		return (get_quoted_content(content));
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

char	*get_quoted_content(char *input) //function working, ommented just to create another and test
{
	char	*res;
	char	quote;
	int		i;
	bool	in_quotes;

	i = 0;
	quote = input[0];
	in_quotes = true;
	while (input[i++])
	{
		if (input[i] == quote)
		{
			if (in_quotes)
				in_quotes = false;
			else
				in_quotes = true;
		}
		if ((input[i] == ' ' || input[i] == '<' || input[i] == '>'
			|| input[i] == '|') && !in_quotes) // it breaks and exists the loop in case it finds any operator and is not between quotes
			break ;
	}
	res = malloc(sizeof(char) * i + 2);
	if (!res)
		return (NULL); // or write any allocation error in the future
	ft_strlcpy(res, input, i + 1);
	return (res);
}

char    *other_content(char *input)
{
    int i;
    int j;
    bool in_quotes;
    char *res;
    int quote;

    i = 0;
    j = 0;
    quote = -1;
    in_quotes = false;
    if (!input)
        return (NULL);
    while (input[j++])
    {
        if (input[j] == '\'' || input[j] == '"')
        {
            quote = j;
            j++;
        }
        else
            j++;
    }
    while (input[i++])
    {
        if (input[i] == quote)
        {
            if (!in_quotes)
                in_quotes = true;
            else
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

// Comandos para testar tipo: echo "$?" $? '$?' "'"$?"'" """'$?'"""

// echo $9$?I$SER$2USER$USER
