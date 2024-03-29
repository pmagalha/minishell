/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 18:35:11 by pmagalha          #+#    #+#             */
/*   Updated: 2024/03/28 09:57:50 by marvin           ###   ########.fr       */
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
		t_lexer	*lexer = NULL;
		printf("\n\033[32;1mLEXER: \n\033[0m");
		lexer = prompt->lexer;
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
			if (!process->command)
			{
				printf("[");
				printf("\033[90m(null)\033[0m");
				printf("]");
			}
			else
			{
				while (process->command)
				{
					printf("[%s]  ", process->command->content);
					process->command = process->command->next;
				}
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
			printf("%s", process->hd_file);
		else		if (process->hd_file)
			printf("%s", process->hd_file);
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
	
	prompt = NULL;
	prompt = init(prompt, env);
	(void)argc;
	set_env(env, prompt);
	while (1)
	{
		if (argv[1])
		{
			printf("Wrong number of arguments\n");
			break ;
		}
		input = readline("minishell$ ");
		if (input != NULL)
		{
			add_history(input);
			
			get_token(input, prompt);
			
			get_parser(prompt);
			dev_mod(prompt);

			prompt->lexer = NULL;
			prompt->parser = NULL;
			free(input);
		}
		else
		{
			exit (1); // isto eh quando faz ctrl D (new line)
			break ;
		}
	}
	free_data(prompt);
	free(prompt);
}

// nao esquecer de fazer o add history nao guardar comandos vazios

/* void	set_sign(void)
{	
	signal.sa_handler = &handle_signal;
	signal.sa_flags = 0;
	sigemptyset(&signal.sa_mask);
	sigaction(SIGINT, &signal, NULL);
	sigaction(SIGQUIT, &signal, NULL);
} */

