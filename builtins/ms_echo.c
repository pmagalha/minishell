/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmagalha <pmagalha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 16:02:02 by pmagalha          #+#    #+#             */
/*   Updated: 2024/04/25 16:28:17 by pmagalha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell/minishell.h"

static void	ft_print(t_lexer *command)
{
	t_lexer	*temp;

	temp = command;
	while (temp)
	{
		ft_putstr_fd(temp->content, STDOUT_FILENO);
		temp = temp->next;
		if (temp)
			ft_putchar_fd(' ', STDOUT_FILENO);
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
		while (temp->content[i] == 'n')
			i++;
		if (temp->content[i] == '\0')
			flg = true;
		else
			break ;
		temp = temp->next;
	}
	ft_print(temp);
	if (flg == false)
	{
		ft_putchar_fd('\n', STDOUT_FILENO);
	}
	return (0);
}
