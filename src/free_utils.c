/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-ppe <joao-ppe@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 16:47:00 by joao-ppe          #+#    #+#             */
/*   Updated: 2024/06/17 18:01:30 by joao-ppe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	free_lexer_list(t_lexer **lexer)
{
	t_lexer	*next;
	t_lexer	*current;

	next = NULL;
	current = *lexer;
	if (lexer == NULL || *lexer == NULL)
		return ;
	while (current != NULL)
	{
		if (current->content)
			free(current->content);
		next = current->next;
		if (current)
			free(current);
		current = next;
	}
	*lexer = NULL;
}

void	free_parser_list(t_parser **parser)
{
	t_parser	*next;
	t_parser	*current;

	next = NULL;
	current = *parser;
	if (!parser || !*parser)
		return ;
	while (current)
	{
		next = current->next;
		if (current->command)
			free_lexer_list(&(current->command));
		if (current->redirects)
			free_lexer_list(&(current->redirects));
		if (current->builtin)
			free(current->builtin);
		if (current->hd_file)
		{
			unlink(current->hd_file);
			free(current->hd_file);
		}
		free(current);
		current = next;
	}
	*parser = NULL;
}

void	free_env_list(t_env_list **env)
{
	t_env_list	*head;
	t_env_list	*tmp;

	head = *env;
	tmp = NULL;
	if (!env)
		return ;
	while (head != NULL)
	{
		if (head->key)
			free(head->key);
		if (head->value)
			free(head->value);
		tmp = head;
		head = head->next;
		free(tmp);
	}
}

void	free_array(char **arr)
{
	int	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
	{
		ms_free_string(arr[i]);
		i++;
	}
	free(arr);
}

void	free_data(t_prompt *prompt)
{
	if (!prompt)
		return ;
	free_env_list(&(prompt->env_list));
	free_lexer_list(&(prompt->lexer));
	free_parser_list(&(prompt->parser));
	if (prompt->pid)
		free(prompt->pid);
	prompt->pid = NULL;
	free(prompt);
}
