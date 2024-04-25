/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmagalha <pmagalha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 12:18:20 by pmagalha          #+#    #+#             */
/*   Updated: 2024/04/25 13:51:57 by pmagalha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	pipe_handling(t_prompt *prompt)
{
	if (prompt->lexer->next->type == PIPE)
		prompt->lexer->next = prompt->lexer->next->next;
	add_parser_back(&(prompt->parser), create_pnode(NULL, NULL, NULL));
	prompt->lexer = prompt->lexer->next;
}
// HA UMA LEAK QUANDO SE FAZ DOIS PIPES EX echo boas || echo top

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
	int	pipe_count;
	t_lexer	*head;

	pipe_count = 0;
	head = lexer;
	while (head != NULL)
	{
		if (head->type == PIPE)
			pipe_count++;
		if ((head->type == PIPE && !head->next)
				|| ((head->next && head->next->next) && head->type == PIPE && head->next->type == PIPE
						&& head->next->next->type == PIPE))
		{
			ft_putstr_fd("minishell: syntax error", STDERR_FILENO);
			ft_putstr_fd("near unexpected token `|'\n", STDERR_FILENO);
			g_code = 2;
			return (-1);
		}
		head = head->next;
	}
	return (pipe_count);
}

char	ms_count_words(t_prompt *prompt)
{
	int		count;
	t_lexer	*temp;

	temp = prompt->lexer;
	count = 0;
	while ((temp && temp->type == OTHER) && get_builtin(prompt) == NULL)
	{
		count++;
		temp = temp->next;
	}
	return (count);
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
			prompt->parser->command = create_node(ft_strdup(prompt->lexer->content), prompt->lexer->type);
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

void	redirects_error(t_prompt *prompt)
{
	prompt->parser->redirects = NULL;
	ft_putstr_fd("bash: syntax error near ", STDERR_FILENO);
	ft_putstr_fd("unexpected token `newline'\n", STDERR_FILENO);
	prompt->lexer = prompt->lexer->next;
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
		if (!prompt->lexer->next)
			return redirects_error(prompt);
		if (!prompt->parser->redirects)
			prompt->parser->redirects = create_node(ft_strdup(prompt->lexer->next->content), prompt->lexer->type);
		else if (prompt->parser->redirects)
		{
			new_node = create_node(ft_strdup(prompt->lexer->next->content), prompt->lexer->type);
			token_add_back(&prompt->parser->redirects, new_node);
		}
		if (prompt->lexer->next->next)
			prompt->lexer = prompt->lexer->next->next;
		else
			prompt->lexer = NULL;
	}
}
