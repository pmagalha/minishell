/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-ppe <joao-ppe@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 18:10:31 by pmagalha          #+#    #+#             */
/*   Updated: 2024/06/17 18:01:38 by joao-ppe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	init_pid(t_prompt *prompt)
{
	t_parser	*parser;
	int			n_parsers;

	parser = prompt->parser;
	n_parsers = 0;
	while (parser)
	{
		n_parsers++;
		parser = parser->next;
	}
	if (!prompt->pid)
		prompt->pid = ft_calloc(sizeof(int), n_parsers);
	else
	{
		free(prompt->pid);
		prompt->pid = ft_calloc(sizeof(int), n_parsers);
	}
	prompt->pid_size = n_parsers;
	if (!prompt->pid)
		return (1);
	return (0);
}

t_prompt	*init(t_prompt *prompt, char **env)
{
	prompt = malloc(sizeof(t_prompt));
	if (!prompt)
		return (NULL);
	prompt->token_lst = NULL;
	prompt->lexer = NULL;
	prompt->parser = NULL;
	prompt->env = env;
	prompt->env_list = NULL;
	prompt->interactive = false;
	prompt->quotes[0] = false;
	prompt->quotes[1] = false;
	prompt->reset = false;
	prompt->pid = NULL;
	return (prompt);
}

t_lexer	*init_lexer(t_lexer *lexer)
{
	lexer = malloc(sizeof(t_lexer));
	if (!lexer)
		return (NULL);
	lexer->type = 0;
	lexer->content = NULL;
	lexer->next = NULL;
	lexer->prev = NULL;
	return (lexer);
}

t_env_list	*init_env(t_env_list *env_list)
{
	env_list = malloc(sizeof(t_env_list));
	if (!env_list)
		return (NULL);
	env_list->key = NULL;
	env_list->value = NULL;
	env_list->next = NULL;
	return (env_list);
}

t_parser	*init_parser(t_parser *parser)
{
	parser = malloc(sizeof(t_parser));
	if (!parser)
		return (NULL);
	parser->command = NULL;
	parser->next = NULL;
	parser->prev = NULL;
	parser->builtin = NULL;
	parser->redirects = NULL;
	parser->hd_file = NULL;
	return (parser);
}
