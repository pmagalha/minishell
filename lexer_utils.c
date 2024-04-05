/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmagalha <pmagalha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 19:28:15 by pmagalha          #+#    #+#             */
/*   Updated: 2024/04/05 10:12:19 by pmagalha         ###   ########.fr       */
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
        return; // Check for NULL pointer
    }

    t_lexer *current = *lexer;
    t_lexer *next = NULL;

    while (current != NULL)
    {
        // Free the content
        //printf("BEFORE FREED CURRENT COMMAND: [%s]\n", current->content);

        if (current->content)
            free(current->content);
        //printf("AFTER FREED CURRENT COMMAND: [%s]\n", current->content);


        // Save the next node before freeing the current one
        next = current->next;

        // Free the current node
        if (current)
            free(current);

        // Move to the next node
        current = next;
    }

    // Update the lexer pointer to NULL
    *lexer = NULL;
}

void free_parser_list(t_parser **parser)
{
    if (parser == NULL || *parser == NULL) {
        return; // Check for NULL pointer
    }

    t_parser *current = *parser;
    t_parser *next = NULL;

    while (current != NULL)
    {
        // Free the command nodes      

        if (current->command)
            free_lexer_list(&(current->command));

        // Free the redirect nodes
        if (current->redirects)
            free_lexer_list(&(current->redirects));

        // Free other members
        //printf("BEFORE FREED BUILTIN: [%s]\n", current->builtin);
        if (current->builtin)
            free(current->builtin);
        //printf("AFTER FREED BUILTIN: [%s]\n", current->builtin);
        if (current->hd_file)
            free(current->hd_file);

        // Save the next node before freeing the current one
        next = current->next;

        // Free the current node
        if (current)
            free(current);
        
        //printf("CURRENT POINTER: [%p]\n", current);

        // Move to the next node
        current = next;
    }
    // Update the parser pointer to NULL
    *parser = NULL;
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

void free_data(t_prompt *prompt)
{
    if (prompt == NULL) {
        return; // Check for NULL pointer
    }

    free_env_list(&(prompt->env_list));
    free_lexer_list(&(prompt->lexer));
    free_parser_list(&(prompt->parser));
    free(prompt);
}
