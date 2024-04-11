/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-ppe <joao-ppe@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 10:37:48 by pmagalha          #+#    #+#             */
/*   Updated: 2024/04/11 13:21:54 by joao-ppe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_parser	*create_pnode(t_lexer *command, char *builtin, t_lexer *redirects)
{
	t_parser	*new;

	new = malloc(sizeof(t_parser));
	if (new == NULL)
		return (NULL);
	new->hd_file = NULL;
	new->command = command;
	new->builtin = builtin;
	new->redirects = redirects;
	new->next = NULL;
	new->prev = NULL;
	return (new);
}

void	add_parser_back(t_parser **token_lst, t_parser *new)
{
	t_parser	*new_node;

	if (*token_lst == NULL)
		*token_lst = new;
	else
	{
		new_node = *token_lst;
		while (new_node->next != NULL)
			new_node = new_node->next;
		new_node->next = new;
		new->prev = new_node;
		*token_lst = new;
	}
}

/* t_parser	*dup_parser(t_prompt *prompt)
{
	t_parser	*parser;

	parser = parser;
	while (parser != NULL) 
	{
		add_parser_back(&parser, create_pnode(parser->command, NULL, NULL));
		printf("Content: [%s] | Builtin:\n\n", prompt->lexer->content);
		parser = head->next;
	}
	return (parser);
}*/

char	*get_builtin(t_prompt *prompt)
{
	if (!prompt->lexer->content)
		return (NULL);
	else if (!ft_strncmp(prompt->lexer->content, "echo", 5))
		return (ft_strdup("echo"));
	else if (!ft_strncmp(prompt->lexer->content, "cd", 3))
		return (ft_strdup("cd"));
	else if (!ft_strncmp(prompt->lexer->content, "pwd", 4))
		return (ft_strdup("pwd"));
	else if (!ft_strncmp(prompt->lexer->content, "export", 7))
		return (ft_strdup("export"));
	else if (!ft_strncmp(prompt->lexer->content, "unset", 6))
		return (ft_strdup("unset"));
	else if (!ft_strncmp(prompt->lexer->content, "env", 4))
		return (ft_strdup("env"));
	else if (!ft_strncmp(prompt->lexer->content, "exit", 5))
		return (ft_strdup("exit"));
	else
		return (NULL);
}
