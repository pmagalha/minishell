/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmagalha <pmagalha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 13:21:11 by pmagalha          #+#    #+#             */
/*   Updated: 2024/04/25 16:05:17 by pmagalha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_code;

int	exec_builtins(t_prompt *prompt, t_parser *parser)
{
	int	status;

	status = 1;
	if (!ft_strncmp(parser->command->content, "echo", 5))
		status = ms_echo(parser);
	else if (!ft_strncmp(parser->command->content, "pwd", 4))
		status = ms_pwd();
	else if (!ft_strncmp(parser->command->content, "env", 4))
		status = ms_env(prompt, parser);
	else if (!ft_strncmp(parser->command->content, "cd", 3))
		status = ms_cd(prompt, parser);
	else if (!ft_strncmp(parser->command->content, "exit", 5))
		return (ms_exit(parser, prompt));
	else if (!ft_strncmp(parser->command->content, "export", 7))
		status = ms_export(prompt, parser);
	else if (!ft_strncmp(parser->command->content, "unset", 6))
		status = ms_unset(prompt, parser);
	return (status);
}
