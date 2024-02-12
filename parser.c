/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 12:18:20 by pmagalha          #+#    #+#             */
/*   Updated: 2024/02/08 19:01:00 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// meter erro para quando so tem redirect e nao tem ficheiro seguinte

void	get_parser(t_prompt *prompt)
{
	int			pipe_count;
	t_lexer		*start;
	t_parser	*p_start;

	start = prompt->lexer;
	if (!prompt->lexer)
		printf("empty lexer");
	add_parser_back(&(prompt->parser), create_pnode(NULL, NULL, NULL));
	p_start = prompt->parser;
	expander(prompt->lexer, prompt->env_list);
	pipe_count = count_pipes(prompt->lexer);
	while (pipe_count-- >= 0)
	{
		while (prompt->lexer && prompt->lexer->type != PIPE)
		{
			check_quotes(prompt->lexer->content);
			prompt->lexer->content = trim_quotes(prompt->lexer->content);
			if (!prompt->parser->builtin)
				prompt->parser->builtin = get_builtin(prompt);
			get_command(prompt);
			get_redirects(prompt);
		}
		if (prompt->parser->command->content)
				exec_builtins(prompt);
		if (prompt->lexer && prompt->lexer->next && prompt->lexer->type == PIPE)
		{
			if (prompt->lexer->next->type == PIPE) // this is the condition for the double pipes case, (it will ignore the second)
				prompt->lexer->next = prompt->lexer->next->next;
			add_parser_back(&(prompt->parser), create_pnode(NULL, NULL, NULL));
			prompt->lexer = prompt->lexer->next;
		}
	}
	prompt->parser = p_start;
	prompt->lexer = start;
}

int	count_pipes(t_lexer *lexer)
{
	int	pipe_count;

	pipe_count = 0;
	while (lexer != NULL)
	{
		if (lexer->type == PIPE)
			pipe_count++;
		if (lexer->type == PIPE && !lexer->next) //condition in case there is only one pipe
		{
			printf("minishell: syntax error near unexpected token `|'\n");
			exit (1);
		}
		lexer = lexer->next;
	}
	return (pipe_count);
}

char	count_words(t_prompt *prompt)
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

void	get_redirects(t_prompt *prompt)
{
	t_lexer	*redirect;

	redirect = NULL;
	if (!prompt->lexer)
		return ;
	if (prompt->lexer->type == REDIR_OUT || prompt->lexer->type == REDIR2_OUT
		|| prompt->lexer->type == REDIR_IN || prompt->lexer->type == HEREDOC)
	{
		if (!prompt->parser->redirects)
			prompt->parser->redirects = create_node(trim_quotes(prompt->lexer->next->content), prompt->lexer->type);
		else
		{
			redirect = prompt->parser->redirects;
			while (redirect->next != NULL)
				redirect = redirect->next;
			redirect->next = malloc(sizeof(t_lexer));
			redirect->next->type = prompt->lexer->type;
			redirect->next->content = trim_quotes(strdup(prompt->lexer->next->content));
			redirect->next->next = NULL;
			redirect->next->prev = redirect->next;
		}
		prompt->lexer = prompt->lexer->next->next;
	}
}
