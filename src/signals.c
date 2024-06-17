/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-ppe <joao-ppe@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 14:26:40 by joao-ppe          #+#    #+#             */
/*   Updated: 2024/06/17 18:02:03 by joao-ppe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

extern int	g_code;

void	execute_signal(void *baseprompt, int sig)
{
	static t_prompt	*prompt;

	if (!prompt && baseprompt)
		prompt = (t_prompt *)baseprompt;
	if (prompt && prompt->interactive == true)
	{
		if (sig == SIGINT)
		{
			write(1, "\n", 1);
			rl_replace_line("", 0);
			rl_on_new_line();
			rl_redisplay();
		}
		else if (sig == SIGQUIT)
		{
			printf("\033[K");
			rl_on_new_line();
			rl_redisplay();
		}
	}
	if (prompt->interactive == false)
		if (sig == SIGINT)
			write(1, "\n", 1);
	if (sig == SIGINT)
		g_code = 130;
}

void	handle_sig(int sig)
{
	execute_signal(0, sig);
}

void	init_sign(void)
{
	struct sigaction	sa;

	sa.sa_handler = &handle_sig;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}

void	set_signals(t_prompt *prompt)
{
	init_sign();
	execute_signal(prompt, 0);
}
