/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmagalha <pmagalha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 16:04:45 by pmagalha          #+#    #+#             */
/*   Updated: 2024/04/25 16:27:21 by pmagalha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell/minishell.h"

int	ms_pwd(void)
{
	char	*temp;

	temp = getcwd(NULL, 0);
	ft_putstr_fd(temp, STDOUT_FILENO);
	ft_putchar_fd('\n', STDOUT_FILENO);
	if (temp)
		free(temp);
	return (0);
}
