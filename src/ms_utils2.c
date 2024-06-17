/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_utils2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-ppe <joao-ppe@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 15:47:37 by pmagalha          #+#    #+#             */
/*   Updated: 2024/06/17 18:01:50 by joao-ppe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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

int	check_pipe(char *content)
{
	int	i;

	i = 1;
	while (content[i] != '|' && content[i])
	{
		if (content[i] != 32 && (content[i] < 9 || content[i] > 13))
			return (1);
		i++;
	}
	return (0);
}
