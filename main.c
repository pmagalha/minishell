/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmagalha <pmagalha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 18:35:11 by pmagalha          #+#    #+#             */
/*   Updated: 2024/01/30 15:30:17 by pmagalha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


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
		printf("\n\033[32;1mLEXER: \033[0m");
		lexer = prompt->lexer;
		while(lexer)
		{
			printf("String: [%s] " "Type: [%s] ", lexer->content, print_type(lexer->type));
			lexer = lexer->next;
		}
	}
	t_parser	*process = prompt->parser;
	int n = 1;
	while (process)
	{
		printf("\n\n\033[32;1mPROCESS (%d): \033[0m", n++);
		printf("\n");
		printf("\033[34m   ARGS: \033[0m");
		if (process->command)
		{
			if (!process->command[0])
			{
				printf("[");
				printf("\033[90m(null)\033[0m");
				printf("]");
			}
			else
			{
				for (int i = 0; process->command[i]; i++)
					printf("[%s] ", process->command[i]);
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
	t_lexer	*currentl;
	t_lexer *current;
	t_parser *currentp;
	t_lexer *next;
	t_prompt *prompt;
	t_env_list *env_list;
	t_env_list *temp;
	
	
	next = NULL;
	prompt = NULL;
	current = NULL;
	currentl = NULL;
	currentp = NULL;
	env_list = NULL;
	prompt = init(prompt, env);
	(void)argc;

	while (1)
	{
		if (argv[1])
		{
			printf("Wrong number of arguments\n");
			break ;
		}
		input = readline("Minishell$ ");
		if (input != NULL)
		{
			add_history(input);
			
			get_token(input, prompt);
			get_env(env, prompt);
			get_parser(prompt);
			
			temp = prompt->env_list; // isto era para testar printar o env na main
			//print_env_list(temp); // funcao para printar a lista de env com todos os seus nodes
			dev_mod(prompt);
			prompt->lexer = NULL;
			prompt->parser = NULL;
			free(input);
		}
		else
		{
			printf("\n"); // isto eh quando faz ctrl D (new line)
		}
	}
	free_parser_list(prompt->parser);
	free_lexer_list(prompt->lexer);
	free(prompt);
}

// nao esquecer de fazer o add history nao guardar comandos vazios
