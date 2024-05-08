/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmagalha <pmagalha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/08 15:41:12 by pmagalha          #+#    #+#             */
/*   Updated: 2024/05/08 15:43:25 by pmagalha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_hd(char *input, int fd)
{
    if (input)
		ft_putstr_fd(input, fd);
	ft_putchar_fd('\n', fd);
}
