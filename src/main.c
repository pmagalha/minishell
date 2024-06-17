/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-ppe <joao-ppe@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 18:35:11 by pmagalha          #+#    #+#             */
/*   Updated: 2024/06/17 18:01:44 by joao-ppe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	g_code = 0;

char	*print_type(t_type type)
{
	if (type == REDIR_IN)
		return ("REDIR_IN");
	else if (type == REDIR_OUT)
		return ("REDIR_OUT");
	else if (type == REDIR2_OUT)
		return ("REDIR2_OUT");
	else if (type == HEREDOC)
		return ("HEREDOC");
	else if (type == PIPE)
		return ("PIPE");
	else
		return ("OTHER");
}

void	the_loop(char *input, t_prompt *prompt)
{
	while (42)
	{
		prompt->interactive = true;
		input = readline("minishell$ ");
		prompt->interactive = false;
		if (!input)
		{
			free_data(prompt);
			exit(0);
		}
		if (*input && check_quotes(input))
		{
			add_history(input);
			get_token(input, prompt);
			if (!has_consecutive_redirects(prompt))
			{
				get_parser(prompt);
				if (!prompt->pid && prompt->parser->next)
					init_pid(prompt);
				execute(prompt);
			}
		}
		ms_free_string(input);
		reset_data(prompt);
	}
}

int	main(int argc, char **argv, char **env)
{
	char		*input;
	t_prompt	*prompt;

	(void)argv;
	input = NULL;
	prompt = NULL;
	if (argc > 1)
	{
		ft_putstr_fd("Minishell does not accept arguments\n", STDERR_FILENO);
		exit (0);
	}
	prompt = init(prompt, env);
	set_signals(prompt);
	set_env(env, prompt);
	the_loop(input, prompt);
	free_data(prompt);
}
