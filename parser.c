/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmagalha <pmagalha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 12:18:20 by pmagalha          #+#    #+#             */
/*   Updated: 2024/01/29 18:48:16 by pmagalha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void   get_parser(t_prompt *prompt)
{
	char	*string;
	char	*builtin;

    //expander(prompt->env_list, prompt->lexer); // expander eh uma funcao que o joao vai fazer, que altera as env variables. 
	if (!prompt->lexer)
	{
		printf("empty lexer");
	}
	t_lexer *current = prompt->lexer; 
	while (current)
	{
		check_quotes(prompt->lexer->content);
		if (!prompt->lexer)
			break ;
		string = trim_quotes(current->content);
		builtin = get_builtin(current->content);
		add_parser_back(&(prompt->parser), create_pnode(string, builtin));
		current = current->next;
	}
    // o parser vai criar um node para cada processo, que depende do numero de pipes. Ao fazer o parser, temos de contar o numero de pipes e assim vamos perceber quantos comandos sao, e temos de criar um node para cada
    // temos de ter um caso no parser para se tiver 3 pipes (echo ||| pwd) dar erro: (syntax error near unexpected token `|')
}

// o expander vai alterar as variaveis do env
// o export eh um built in que vai fazer o mesmo

int check_quotes(char *str)
{
    int num_squotes = 0;
    int num_dquotes = 0;

    while (*str)
    {
        if (*str == '\'')
            num_squotes++;
        else if (*str == '"')
            num_dquotes++;

        str++;
	}
    if (num_squotes % 2 == 0 && num_dquotes % 2 == 0)
        return (0);
    else
	{
		printf("!!!!!!!!!!! Error in number of quotes !!!!!!!!!!!!!!!");
        return (1);
	}
}

char	get_first_quote(char *line)
{
	while (*line)
	{
		if (*line == '\'' || *line == '\"')
			return (*line);
		line++;
	}
	return (*line);
}

char	*trim_quotes(char *string)
{
	char	*res;
	char	quote;
	int		i;
	int		j;
	int		count;

	quote = get_first_quote(string);
	i = -1;
	count = 0;
	while (string[++i])
		if (string[i] == quote)
			count++;
	res = ft_calloc(sizeof(char), i - count + 1);
	i = 0;
	j = 0;
	while (string[i])
	{
		if (string[i] == quote)
			i++;
		else
			res[j++] = string[i++];
	}
	res[j] = '\0';
	return (res);
}

t_parser *create_pnode(void *string, char *builtin) 
{
    t_parser *new;
	
	new = malloc(sizeof(t_parser));
    if (new == NULL)
        return NULL;
    new->string = string;
    new->builtin = builtin;
    new->next = NULL;
    new->prev = NULL;
    return (new);
}

void    add_parser_back(t_parser **token_lst, t_parser *new)
{
	t_parser	*new_node;

	if (*token_lst == NULL)
		*token_lst = new;
	else
	{
		new_node = *token_lst;
		while (new_node->next != NULL)
		{
			new_node = new_node->next;
		}
			
		new_node->next = new;
		new->prev = new_node;
	}
}

t_parser *dup_parser(t_lexer *lexer)
 {
    t_lexer *head = lexer;
	t_parser *parser = NULL;

    while (head != NULL) 
	{
        add_parser_back(&parser, create_pnode(head->content, NULL));
		printf("Content: [%s] | Builtin:\n\n", head->content);
		head = head->next;
    }
    return (parser);
}

char    *get_builtin(char *content)
{
    if (!ft_strncmp(content, "echo", 4))
        return (ft_strdup("echo"));
    else if (!ft_strncmp(content, "cd", 2))
        return (ft_strdup("cd"));
    else if (!ft_strncmp(content, "pwd", 3))
        return (ft_strdup("pwd"));
    else if (!ft_strncmp(content, "export", 6))
        return (ft_strdup("export"));
    else if (!ft_strncmp(content, "unset", 5))
        return (ft_strdup("unset"));
    else if (!ft_strncmp(content, "env", 3))
        return (ft_strdup("env"));
    else if (!ft_strncmp(content, "exit", 4))
        return (ft_strdup("exit"));
    else
        return (ft_strdup("string"));
}

// depois temos de fazer tambem condicoes nesta funcao para redirection e heredocs e tratar do dollar sign (expander)