/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmagalha <pmagalha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 19:28:15 by pmagalha          #+#    #+#             */
/*   Updated: 2024/04/18 12:24:28 by pmagalha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_lexer	*create_node(char *content, t_type type)
{
	t_lexer	*new;

	new = (t_lexer *)malloc(sizeof(t_lexer));
	if (new == NULL)
		return (NULL);
	new->content = ft_strdup(content);
	new->type = type;
	new->next = NULL;
	new->prev = NULL;
    free(content);
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

void free_lexer_list(t_lexer **lexer)
{
    if (lexer == NULL || *lexer == NULL) {
        return;
    }
    t_lexer *current = *lexer;
    t_lexer *next = NULL;

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

void free_parser_list(t_parser **parser)
{
    if (parser == NULL || *parser == NULL) {
        return;
    }

    t_parser *current = *parser;
    t_parser *next = NULL;

    while (current != NULL)
    {
        if (current->command)
            free_lexer_list(&(current->command));
        if (current->redirects)
            free_lexer_list(&(current->redirects));
        if (current->builtin)
            free(current->builtin);
        if (current->hd_file)
            free(current->hd_file);
        next = current->next;
        if (current)
            free(current);
        current = next;
    }
    *parser = NULL;
}

void free_env_list(t_env_list **env)
{
    t_env_list *head;
    t_env_list *tmp;

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
    //free(env);
}

void free_data(t_prompt *prompt)
{
    if (!prompt)
        return;
    free_env_list(&(prompt->env_list));
    free_lexer_list(&(prompt->lexer));
    free_parser_list(&(prompt->parser));
    if (prompt->pid)
        free(prompt->pid);
    prompt->pid = NULL;
    free(prompt);
}
