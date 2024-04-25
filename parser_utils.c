/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmagalha <pmagalha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 10:37:48 by pmagalha          #+#    #+#             */
/*   Updated: 2024/04/25 17:39:00 by pmagalha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_parser	*create_pnode(t_lexer *command, char *builtin, t_lexer *redirects)
{
	t_parser	*new;

	new = malloc(sizeof(t_parser));
	if (new == NULL)
		return (NULL);
	new->hd_file = NULL;
	new->command = command;
	new->builtin = builtin;
	new->redirects = redirects;
	new->next = NULL;
	new->prev = NULL;
	return (new);
}

void	add_parser_back(t_parser **token_lst, t_parser *new)
{
	t_parser	*new_node;

	if (*token_lst == NULL)
		*token_lst = new;
	else
	{
		new_node = *token_lst;
		while (new_node->next != NULL)
			new_node = new_node->next;
		new_node->next = new;
		new->prev = new_node;
		*token_lst = new;
	}
}

char	ms_count_words(t_prompt *prompt)
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

void	redirects_error(t_prompt *prompt)
{
	prompt->parser->redirects = NULL;
	ft_putstr_fd("bash: syntax error near ", STDERR_FILENO);
	ft_putstr_fd("unexpected token `newline'\n", STDERR_FILENO);
	prompt->lexer = prompt->lexer->next;
}

char	*get_builtin(t_prompt *prompt)
{
	if (!prompt->lexer->content)
		return (NULL);
	else if (!ft_strncmp(prompt->lexer->content, "echo", 5))
		return (ft_strdup("echo"));
	else if (!ft_strncmp(prompt->lexer->content, "cd", 3))
		return (ft_strdup("cd"));
	else if (!ft_strncmp(prompt->lexer->content, "pwd", 4))
		return (ft_strdup("pwd"));
	else if (!ft_strncmp(prompt->lexer->content, "export", 7))
		return (ft_strdup("export"));
	else if (!ft_strncmp(prompt->lexer->content, "unset", 6))
		return (ft_strdup("unset"));
	else if (!ft_strncmp(prompt->lexer->content, "env", 4))
		return (ft_strdup("env"));
	else if (!ft_strncmp(prompt->lexer->content, "exit", 5))
		return (ft_strdup("exit"));
	else
		return (NULL);
}
