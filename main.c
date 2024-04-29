/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmagalha <pmagalha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 18:35:11 by pmagalha          #+#    #+#             */
/*   Updated: 2024/04/29 13:33:21 by pmagalha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int	dev_mod(t_prompt *prompt)
{
	printf("\033[32;1m=========== DEV MOD ==========\033[0m\n");
	if (prompt->lexer)
	{
		t_lexer	*lexer = prompt->lexer;
		printf("\n\033[32;1mLEXER: \n\033[0m");
		while(lexer)
		{
			printf("String: [%s] " "Type: [%s] \n", lexer->content, print_type(lexer->type));
			lexer = lexer->next;
		}
	}
	t_parser	*process = prompt->parser;
	int n = 1;
	while (process)
	{
		printf("\n\n\033[32;1mPARSER (%d): \033[0m", n++);
		printf("\n");
		printf("\033[34m   COMMANDS: \033[0m");
		if (process->command)
		{
			t_lexer *temp_command = process->command; // Create a temporary pointer
			while (temp_command)
			{
				printf("[%s]  ", temp_command->content);
				temp_command = temp_command->next; // Move the temporary pointer
			}
		}
		else
			printf("\033[90m(null)\033[0m");
		printf("\n\033[34m   BUILTIN: \033[0m");
		if (process->builtin)
			printf("[%s]\n", process->builtin);
		else
			printf("\033[90m(null)\033[0m\n");
		printf("\033[34m   REDIRECT: \033[0m");
		t_lexer *redir = process->redirects;
		if (!redir)
			printf("\033[90m(null)\033[0m");
		else
		{
			while (redir)
			{
				if (redir->type == REDIR_IN)
					printf("[<] ");
				if (redir->type == REDIR_OUT)
					printf("[>] ");
				if (redir->type == REDIR2_OUT)
					printf("[>>] ");
				if (redir->type == HEREDOC)
					printf("[<<] ");
				printf("[%s]  ", redir->content);
				redir = redir->next;
			}
		}
		printf("\n\033[34m   HD_FILE: \033[0m");
		if (process->hd_file)
			printf("[%s]", process->hd_file);
		else
			printf("\033[90m(null)\033[0m");
		process = process->next;
	}
	printf("\n\n");
	printf("\033[32;1m==============================\033[0m\n");
	return (1);
}

/* ============================  dev mod ============================ */

int main(int argc, char **argv, char **env)
{
    char *input;
	t_prompt *prompt;
	
	(void)argv;
	prompt = NULL;
	if (argc > 1)
	{
		ft_putstr_fd("Minishell does not accept arguments\n", STDERR_FILENO);
		exit (0);
	}
	prompt = init(prompt, env);
	set_signals(prompt);
	set_env(env, prompt);
	while (1)
	{
		prompt->interactive = true;
		input = readline("minishell$ ");
		prompt->interactive = false;
		if (input != NULL)
		{
			add_history(input);
			if (!check_quotes(input))
			{
				reset_data(prompt);
				continue;
			}
			get_token(input, prompt);
			if (has_consecutive_redirects(prompt, input))
				continue ;
			get_parser(prompt);
			//dev_mod(prompt); // aapaaaagare
			if (!prompt->pid && prompt->parser->next)
				init_pid(prompt);
			execute(prompt);
			free(input);
		}
		else
		{
			free_data(prompt);
			exit (1); // isto eh quando faz ctrl D (new line)
		}
		reset_data(prompt);
	}
	free_data(prompt);
}
