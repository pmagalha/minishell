/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 13:21:11 by pmagalha          #+#    #+#             */
/*   Updated: 2024/02/05 16:27:47 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	pwd(void)
{
	printf("%s\n", (getcwd(NULL, 0)));
}

void	echo(t_prompt *prompt)
{
	t_lexer	*current;

	current = prompt->lexer;
	if (!current)
	{
		printf("No content in the lexer.\n");
		return ;
	}
	current = current->next;
	while (current && current->type == OTHER)
	{
		if (current->content)
			printf("%s", current->content);
		else
			printf("NULL");
		if (current->next && current->next->content)
			printf(" ");
		current = current->next;
	}
	printf("\n");
} // sera preciso acrescentar algo para ignorar /n

void	env_builtin(t_prompt *prompt)
{
	t_env_list	*current;

	current = prompt->env_list;
	while (current != NULL)
	{
		printf("%s=%s\n", current->key, current->value);
		current = current->next;
	}
}

void	exec_builtins(t_prompt *prompt)
{
	if (!ft_strncmp(prompt->lexer->content, "echo", 5))
		echo(prompt);
	/*else if (!ft_strncmp(prompt->lexer->content, "cd", 3))
		cd();*/
	else if (!ft_strncmp(prompt->lexer->content, "pwd", 4))
		pwd();
	/*else if (!ft_strncmp(prompt->lexer->content, "export", 7))
		export();
	else if (!ft_strncmp(prompt->lexer->content, "unset", 6))
		unset();*/
	else if (!ft_strncmp(prompt->lexer->content, "env", 4))
		env_builtin(prompt); 
	//else (!ft_strncmp(prompt->lexer->content, "exit", 5))
	//	exit();
}
