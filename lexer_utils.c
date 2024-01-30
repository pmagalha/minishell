/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmagalha <pmagalha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 19:28:15 by pmagalha          #+#    #+#             */
/*   Updated: 2024/01/30 13:42:16 by pmagalha         ###   ########.fr       */
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
        free(current->content);
		// free(current->type); ver se funciona com isto
        free(current);
        current = next;
    }
}

void	free_parser_list(t_parser *head)
{
    t_parser	*current;
    t_parser	*next;

	current = head;
    while (current != NULL)
    {
        next = current->next;
        free(current->command);
		free(current->builtin);
        free(current);
        current = next;
    }
}