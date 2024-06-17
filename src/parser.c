/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-ppe <joao-ppe@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 12:18:20 by pmagalha          #+#    #+#             */
/*   Updated: 2024/06/17 18:01:55 by joao-ppe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	pipe_handling(t_prompt *prompt)
{
	t_lexer	*tmp;

	tmp = prompt->lexer;
	if (prompt->lexer->next->type == PIPE)
	{
		tmp = prompt->lexer;
		prompt->lexer = prompt->lexer->next;
	}
	add_parser_back(&(prompt->parser), create_pnode(NULL, NULL, NULL));
	prompt->lexer = prompt->lexer->next;
}

void	get_parser(t_prompt *prompt)
{
	int			pipe_count;
	t_lexer		*start;
	t_parser	*p_start;

	start = prompt->lexer;
	add_parser_back(&(prompt->parser), create_pnode(NULL, NULL, NULL));
	p_start = prompt->parser;
	pipe_count = count_pipes(prompt->lexer);
	if (pipe_count < 0)
		return ;
	while (pipe_count-- >= 0)
	{
		while (prompt->lexer && prompt->lexer->type != PIPE)
		{
			if (!prompt->parser->builtin)
				prompt->parser->builtin = get_builtin(prompt);
			get_command(prompt);
			get_redirects(prompt);
		}
		if (prompt->lexer && prompt->lexer->next && prompt->lexer->type == PIPE)
			pipe_handling(prompt);
	}
	prompt->parser = p_start;
	prompt->lexer = start;
}

int	count_pipes(t_lexer *lexer)
{
	int		pipe_count;
	t_lexer	*head;

	pipe_count = 0;
	head = lexer;
	while (head != NULL)
	{
		if (head->type == PIPE)
		{
			if (head == lexer || (head->prev && head->prev->type == PIPE))
			{
				return (pipe_error());
				exit (g_code);
			}
			pipe_count++;
		}
		if ((head->type == PIPE && !head->next)
			|| ((head->next && head->next->next) && head->type == PIPE
				&& head->next->type == PIPE
				&& head->next->next->type == PIPE))
			return (pipe_error());
		head = head->next;
	}
	return (pipe_count);
}

void	get_command(t_prompt *prompt)
{
	t_lexer	*command_node;

	command_node = NULL;
	if (!prompt->lexer)
		return ;
	if (prompt->lexer->type == OTHER)
	{
		if (!prompt->parser->command)
			prompt->parser->command = create_node(ft_strdup
					(prompt->lexer->content), prompt->lexer->type);
		else
		{
			command_node = prompt->parser->command;
			while (command_node->next != NULL)
				command_node = command_node->next;
			command_node->next = malloc(sizeof(t_lexer));
			command_node->next->type = prompt->lexer->type;
			command_node->next->content = ft_strdup(prompt->lexer->content);
			command_node->next->next = NULL;
			command_node->next->prev = command_node;
		}
		prompt->lexer = prompt->lexer->next;
	}
}

void	get_redirects(t_prompt *prompt)
{
	t_lexer	*redirect;
	t_lexer	*new_node;

	redirect = NULL;
	new_node = NULL;
	if (!prompt->lexer)
		return ;
	if (prompt->lexer->type == REDIR_OUT || prompt->lexer->type == REDIR2_OUT
		|| prompt->lexer->type == REDIR_IN || prompt->lexer->type == HEREDOC)
	{
		if (!prompt->parser->redirects)
			prompt->parser->redirects = create_node(ft_strdup
					(prompt->lexer->next->content), prompt->lexer->type);
		else if (prompt->parser->redirects)
		{
			new_node = create_node(ft_strdup
					(prompt->lexer->next->content), prompt->lexer->type);
			token_add_back(&prompt->parser->redirects, new_node);
		}
		if (prompt->lexer->next->next)
			prompt->lexer = prompt->lexer->next->next;
		else
			prompt->lexer = NULL;
	}
}
