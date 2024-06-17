/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-ppe <joao-ppe@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/08 15:41:12 by pmagalha          #+#    #+#             */
/*   Updated: 2024/06/17 18:01:32 by joao-ppe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	print_hd(char *input, int fd)
{
	if (input)
		ft_putstr_fd(input, fd);
	ft_putchar_fd('\n', fd);
}
