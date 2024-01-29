/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmagalha <pmagalha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 18:35:11 by pmagalha          #+#    #+#             */
/*   Updated: 2024/01/29 18:53:29 by pmagalha         ###   ########.fr       */
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

static void	dev_mod(t_prompt *prompt)
{
	t_parser		*cmds;
	int				j;
	int				i;
	
	printf("\n\033[1;32m* LEXER *\033[0m");
	printf("\n\033[1;32m--------------------------------------\033[0m\n");
	for (t_lexer *node = prompt->lexer; node; node = node->next)
	{
		if (node->content)
			printf("Content: [%s] | Type: [%s]\n", node->content, print_type(node->type));
	}
	printf("\n\n\n\033[1;32m* PARSER *\033[0m");
	printf("\n\033[1;32m--------------------------------------\033[0m\n");
	
  	cmds = prompt->parser;
	j = 0;
	while (cmds)
	{
		printf("\nPROCESS[%d]:\n", ++j);
		printf("\n  \033[0;34mSTR:\033[0m ");
		i = -1;
		printf("[%s] ", cmds->string);
		printf("\n  \033[0;34mBUILTIN:\033[0m %s\n", cmds->builtin);
		//printf("  \033[0;34mREDIRCT NUMBER:\033[0m %d\n", cmds->num_redirct);
		//printf("  \033[0;34mREDIRCT:\033[0m ");
		
/* 		node = cmds->redirct;
		while (node)
		{
			if (node->token)
				printf("[%s]", node->token);
			if (node->str)
				printf("[%s]", node->str);
			printf("  ");
			node = node->next;
		} */
		printf("\n");
		cmds = cmds->next;
		
	}
	printf("\n\033[1;32m* OUTPUT *\033[0m");
	printf("\n\033[1;32m--------------------------------------\033[0m\n");
}

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
		
			current = prompt->lexer;
			
			temp = prompt->env_list; // isto era para testar printar o env na main
			//print_env_list(temp); // funcao para printar a lista de env com todos os seus nodes
			
			dev_mod(prompt);
			prompt->lexer = NULL;
			free(input);
		}
		else
		{
			printf("\n"); // isto eh quando faz ctrl D (new line)
		}
	}
	free_token_list(prompt->lexer);
	free(prompt);
}

// nao esquecer de fazer o add history nao guardar comandos vazios
