/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmagalha <pmagalha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 18:10:31 by pmagalha          #+#    #+#             */
/*   Updated: 2024/01/30 13:42:43 by pmagalha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	env_list->full_string = NULL;
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