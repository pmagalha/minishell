/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-ppe <joao-ppe@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 16:02:02 by pmagalha          #+#    #+#             */
/*   Updated: 2024/06/17 18:00:12 by joao-ppe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	ft_print(t_lexer *command)
{
	t_lexer	*temp;
	char	*homedir;

	homedir = NULL;
	temp = command;
	while (temp)
	{
		if (!ft_strncmp(temp->content, "~", 2))
			homedir = ft_strdup(getenv("HOME"));
		if (homedir)
			ft_putstr_fd(homedir, STDOUT_FILENO);
		else
			ft_putstr_fd(temp->content, STDOUT_FILENO);
		temp = temp->next;
		if (temp || homedir)
			ft_putchar_fd(' ', STDOUT_FILENO);
		ms_free_string(homedir);
		homedir = NULL;
	}
}

int	ms_echo(t_parser *parser)
{
	int		i;
	float	flg;
	t_lexer	*temp;

	i = 1;
	flg = false;
	temp = parser->command->next;
	while (temp && temp->content[0] == '-' && temp->content[1] == 'n')
	{
		i = 1;
		while (temp->content[i] == 'n')
			i++;
		if (!temp->content[i])
			flg = true;
		else
			break ;
		temp = temp->next;
	}
	ft_print(temp);
	if (flg == false)
		ft_putchar_fd('\n', STDOUT_FILENO);
	return (0);
}
