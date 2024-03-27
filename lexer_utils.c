/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-ppe <joao-ppe@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 19:28:15 by pmagalha          #+#    #+#             */
/*   Updated: 2024/03/27 18:43:41 by joao-ppe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_lexer	*create_node(void *content, t_type type)
{
	t_lexer	*new;

	new = (t_lexer *)malloc(sizeof(t_lexer));
	if (new == NULL)
		return (NULL);
	new->content = content;
	new->type = type;
	new->next = NULL;
	new->prev = NULL;
	return (new);
}

void	token_add_back(t_lexer **token_lst, t_lexer *new)
{
	t_lexer	*new_node;

	if (*token_lst == NULL)
		*token_lst = new;
	else
	{
		new_node = *token_lst;
		while (new_node->next != NULL)
			new_node = new_node->next;
		new_node->next = new;
	}
}

void	free_lexer_list(t_lexer *head)
{
	t_lexer	*current;
	t_lexer	*next;

	current = head;
	while (current != NULL)
	{
		next = current->next;
		if (current->content)
			free(current->content);
		free(current);
		current = next;
	}
}

void	free_parser_list(t_parser *head)
{
	t_parser	*current;
	t_parser	*next;

	if (!head)
		return ;
	current = head;
	while (current != NULL)
	{
		next = current->next;
		if (current->command)
			free_lexer_list(current->command);
		if (current->builtin)
			free(current->builtin);
		if (current->redirects)
			free_lexer_list(current->redirects);
		if (current->hd_file)
			free (current->hd_file);
		free(current);
		current = next;
	}
}

void free_env_list(t_env_list *head)
{
    t_env_list *current;
    t_env_list *next;

    current = head;
    while (current != NULL) 
	{
        next = current->next;
        if (current->key)
            free(current->key);
        if (current->value)
            free(current->value);
        if (current->full_string)
            free(current->full_string);
        free(current);
        current = next;
    }
}

void	free_data(t_prompt *prompt)
{
	free_env_list(prompt->env_list);
	free_lexer_list(prompt->lexer);
	free_parser_list(prompt->parser);
}
