/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-ppe <joao-ppe@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 14:26:40 by joao-ppe          #+#    #+#             */
/*   Updated: 2024/04/24 16:50:02 by joao-ppe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_code;

void	execute_sig(void *baseprompt, int sig)
{
	static t_prompt	*prompt;

	if (!prompt && baseprompt)
		prompt = (t_prompt *)baseprompt;
	if (prompt && prompt->interactive)
	{
		if (sig == SIGINT) // ctrl d
		{
			//printf("ENTREI CARALHO 1\n");
			write(1, "\n", 1);
			rl_replace_line("", 0);
			rl_on_new_line();
			rl_redisplay();
			
		}
		else if (sig == SIGQUIT) // ctl C
		{
			//printf("ENTREI CARALHO 2\n");
			printf("\033[K");
			rl_on_new_line();
			rl_redisplay();
			//exit(130);
			//printf("\nexit\n");
		}
	}
	if (prompt->interactive == false)
		if (sig == SIGINT)
			write(1, "\n", 1);
	if (sig == SIGINT)
		g_code = 130;
}

void	handle(int sig)
{
	execute_sig(0, sig);
}

void	init_sign(void)
{	
	struct sigaction	sa;
	
	sa.sa_handler = &handle;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}
void	set_signals(t_prompt *prompt)
{
	init_sign();
	execute_sig(prompt, 0);
}