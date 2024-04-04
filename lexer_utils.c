/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-ppe <joao-ppe@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 19:28:15 by pmagalha          #+#    #+#             */
/*   Updated: 2024/04/04 14:26:51 by joao-ppe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_lexer	*create_node(char *content, t_type type)
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

void free_lexer_list(t_lexer **lexer)
{
    t_lexer *head;
	t_lexer	*tmp;

	head = *lexer;
	tmp = NULL;
    while (head != NULL)
    {
        // Free the content first
        if (head->content != NULL)
        {
            //printf("Limpei o current content: [%s]\n", head->content);
            free(head->content);
			//printf("Limpei o current content AFTER: [%s]\n", head->content);
        }
        // Now free the current node
        //printf("Limpei o CURRENT: [%p]\n", head);
		tmp = head;
		head = head->next;
        free(tmp);
		//printf("Limpei o CURRENT POST FREE	: [%p]\n", head);
    }
}

void free_parser_list(t_parser **parser)
{
	t_parser *head;
    t_parser *tmp;
	
	head = *parser;
	tmp = NULL;
    while (head != NULL)
    {
        if (head->command)
			free_lexer_list(&head->command);
		//printf("FREED PARSER | HEAD->COMMAND: [%s]\n", head->command->content);
        // Free dos redirect nodes
        if (head->redirects)
            free_lexer_list(&head->redirects);
		//printf("FREED PARSER | HEAD->REDIRECTS: [%s]\n", head->redirects->content);
		if (head->builtin)
        	free(head->builtin); // Libertar memoria allocada para os builtins
		//printf("FREED PARSER | HEAD->BUILTIN: [%s]\n", head->builtin);
        if (head->hd_file)
			free(head->hd_file); // Libertar memoria allocada para os hd_files (wtv that is)
        tmp = head;
		//if (tmp != NULL)
		//	free (tmp);
		head = head->next;
    }
}

void free_env_list(t_env_list **env)
{
    t_env_list *head;
    t_env_list *tmp;

    head = *env;
	tmp = NULL;
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

void	free_data(t_prompt *prompt)
{
	if (!prompt)
		return ;
	free_env_list(&prompt->env_list);
	free_lexer_list(&prompt->lexer);
	free_parser_list(&prompt->parser);
}
