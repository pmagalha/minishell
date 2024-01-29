/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmagalha <pmagalha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 18:09:16 by pmagalha          #+#    #+#             */
/*   Updated: 2024/01/29 18:01:44 by pmagalha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    get_token(char *input, t_prompt *prompt)
{
    char    *content;
    t_type  type;
    
    while (*input)
    {
        while (*input == ' ')
            input++;
        if (!*input)
            break ;
        content = get_token_content(input);
        type = get_type(content);
        input += ft_strlen(content);
        token_add_back(&(prompt->lexer), create_node(content, type));
    }
}

char    *get_token_content(char *content)
{
    if (*content == '>' || *content == '<' || *content == '|')
        return (get_operator(content));
    else if (*content == '"' || *content == '\'')
        return (get_quoted_content(content));
    else
        return (other_content(content));
}

char    *get_operator(char *content)
{
    int i;

    i = 0;
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

t_type  get_type(char *content)
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

char    *get_quoted_content(char *input) //function working, ommented just to create another and test
{
    char    *res;
    char    quote;
    int     i;
    bool in_quotes;

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


