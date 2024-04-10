/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-ppe <joao-ppe@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 18:35:11 by pmagalha          #+#    #+#             */
/*   Updated: 2024/04/10 15:44:48 by joao-ppe         ###   ########.fr       */
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

void	reset_data(t_prompt *prompt)
{
	if (!prompt)
		return ;
	if (prompt->lexer)
		free_lexer_list(&prompt->lexer);
	if (prompt->parser)
		free_parser_list(&prompt->parser);
	prompt->lexer = NULL;
	prompt->parser = NULL;
}

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
			exit (2);
		}
		input = readline("minishell$ ");
		if (input != NULL)
		{
			add_history(input);
			get_token(input, prompt);
			get_parser(prompt);
			dev_mod(prompt); // aapaaaagare
			single_command(prompt, prompt->parser);
			free(input);
		}
		else
		{
			free_data(prompt);
			exit (1); // isto eh quando faz ctrl D (new line)
		}
		reset_data(prompt);
		//printf("AFTER RESET DATA:\n");
    	//print_parser(prompt);
		
		//prompt->lexer = NULL;
		//prompt->parser = NULL;
	}
	free_data(prompt);
}

// EXCLUIR LEAKS DO READLINE: valgrind -s --suppressions=readline_leaks.txt --leak-check=full --show-leak-kinds=all ./minishell

// nao esquecer de fazer o add history nao guardar comandos vazios

/* void	set_sign(void)
{	
	signal.sa_handler = &handle_signal;
	signal.sa_flags = 0;
	sigemptyset(&signal.sa_mask);
	sigaction(SIGINT, &signal, NULL);
	sigaction(SIGQUIT, &signal, NULL);
} */

