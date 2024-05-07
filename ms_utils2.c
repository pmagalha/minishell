/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_utils2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmagalha <pmagalha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 15:47:37 by pmagalha          #+#    #+#             */
/*   Updated: 2024/05/07 15:55:41 by pmagalha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exit_builtin(t_prompt *prompt, t_parser *parser)
{
	g_code = exec_builtins(prompt, parser);
	free_data(prompt);
	exit (g_code);
}

void	update_shlvl(t_prompt *prompt)
{
	t_env_list	*head;
	int			shlvl;
	char		*value;

	value = get_env(prompt, "SHLVL");
	if (!value)
		return ;
	shlvl = ft_atoi(value);
	head = prompt->env_list;
	while (head)
	{
		if (!ft_strncmp(head->key, "SHLVL", ft_strlen(head->key) + 1))
		{
			ms_free_string(head->value);
			head->value = ft_itoa(shlvl + 1);
		}
		head = head->next;
	}
}

char	*get_hd_input(t_prompt *prompt)
{
	char	*input;

	input = NULL;
	prompt->interactive = true;
	input = readline("> ");
	prompt->interactive = false;
	return (input);
}
