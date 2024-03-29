/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-ppe <joao-ppe@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 19:28:15 by pmagalha          #+#    #+#             */
/*   Updated: 2024/03/29 13:16:58 by joao-ppe         ###   ########.fr       */
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

void free_lexer_list(t_lexer *head)
{
    t_lexer *current;
    t_lexer *next;

    current = head;
    while (current != NULL)
    {
        next = current->next;
        if (current->content) // libertar content alocado pela str_dup
            free(current->content);
        free(current);
        current = next;
    }
}

void free_parser_list(t_parser *head)
{
    t_lexer *temp;
	t_parser *parser;
    t_parser *next_parser;
	
	parser = head;
	next_parser = parser->next;
    while (parser != NULL)
    {
        // Free dos nodes
        while (parser->command != NULL)
        {
            temp = parser->command;
            parser->command = parser->command->next;
            free(temp->content); // libertar content alocado pela str_dup
            free(temp);
        }
        // Free dos redirect nodes
        while (parser->redirects != NULL)
        {
            temp = parser->redirects;
            parser->redirects = parser->redirects->next;
            free(temp->content); // libertar content alocado pela str_dup
            free(temp);
        }
		
        free(parser->builtin); // Libertar memoria allocada para os builtins
        free(parser->hd_file); // Libertar memoria allocada para os hd_files (wtv that is)

        free(parser);
        parser = next_parser;
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
