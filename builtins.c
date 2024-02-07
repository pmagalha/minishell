/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 13:21:11 by pmagalha          #+#    #+#             */
/*   Updated: 2024/02/07 14:40:59 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	pwd(void)
{
	ft_putstr_fd((getcwd(NULL, 0)), STDOUT_FILENO);
	ft_putchar_fd('\n', STDOUT_FILENO);
}

void	echo(t_parser *parser)
{
	t_lexer	*temp;
	bool	suppress_newline ;

	suppress_newline = false;
	temp = parser->command->next; // incrementa para passar a frente o echo
	while (temp != NULL) 
	{
    	if (strcmp(temp->content, "-n") == false) 
			suppress_newline = true;  // mete a flag para dar suppress da newline 
		else 
		{
    		ft_putstr_fd(temp->content, STDOUT_FILENO);
			if (temp->next != NULL)
    			ft_putchar_fd(' ', STDOUT_FILENO);
		}
		temp = temp->next;
	}
	if (!suppress_newline)
        ft_putchar_fd('\n', STDOUT_FILENO);
	free(temp);
}

void	env_builtin(t_prompt *prompt)
{
	t_env_list	*current;

	current = prompt->env_list;
	while (current != NULL)
	{
		ft_putstr_fd(current->key, STDOUT_FILENO);
		ft_putchar_fd('=', STDOUT_FILENO);
		ft_putstr_fd(current->value, STDOUT_FILENO);
		ft_putchar_fd('\n', STDOUT_FILENO);
		current = current->next;
	}
}

void	cd(t_prompt *prompt)
{
	t_lexer	*temp;

	// aqui tem de ter um home = HOME (variavel do env)
	temp = prompt->parser->command->next;
	if (!temp || !temp->content[0] || (temp->content[0]
			&& !ft_strncmp(temp->content, "~", 2) && !temp->content[1]))
		printf("HOME\n"); // isto tem de ir buscar ao env e atualizar o env, tem de atualizar o PWD e o OLDPWD
}

void	exec_builtins(t_prompt *prompt)
{
	if (!ft_strncmp(prompt->parser->command->content, "echo", 5))
		echo(prompt->parser);
	else if (!ft_strncmp(prompt->parser->command->content, "pwd", 4))
		pwd();
	else if (!ft_strncmp(prompt->parser->command->content, "env", 4))
		env_builtin(prompt); 
	else if (!ft_strncmp(prompt->parser->command->content, "cd", 3))
		cd(prompt);
	/*else if (!ft_strncmp(prompt->lexer->content, "export", 7))
		export();
	else if (!ft_strncmp(prompt->lexer->content, "unset", 6))
		unset();*/
	//else (!ft_strncmp(prompt->lexer->content, "exit", 5))
	//	exit();
}
