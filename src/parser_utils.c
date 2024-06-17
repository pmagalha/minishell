/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-ppe <joao-ppe@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 10:37:48 by pmagalha          #+#    #+#             */
/*   Updated: 2024/06/17 18:01:53 by joao-ppe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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

bool	redirects_error(t_prompt *prompt, char *msg)
{
	t_lexer	*current;

	current = prompt->lexer;
	while (current)
	{
		if (current->type == REDIR_OUT || current->type == REDIR2_OUT
			|| current->type == REDIR_IN || current->type == HEREDOC)
		{
			if (!current->next || (current->next->type >= REDIR_OUT
					&& current->next->type <= HEREDOC))
			{
				ft_putstr_fd(msg, STDERR_FILENO);
				if (!current->next)
					ft_putstr_fd("`newline'\n", STDERR_FILENO);
				else
				{
					ft_putstr_fd(current->next->content, STDERR_FILENO);
					ft_putstr_fd("'\n", STDERR_FILENO);
				}
				return (true);
			}
		}
		current = current->next;
	}
	return (false);
}

bool	has_consecutive_redirects(t_prompt *prompt)
{
	char	*msg;

	msg = ft_strdup("minishell: syntax error near unexpected token ");
	if (redirects_error(prompt, msg))
	{
		reset_data(prompt);
		return (ms_free_string(msg), true);
	}
	return (ms_free_string(msg), false);
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
