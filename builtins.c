/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 13:21:11 by pmagalha          #+#    #+#             */
/*   Updated: 2024/02/06 19:33:14 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	pwd(void)
{
	printf("%s\n", (getcwd(NULL, 0)));
}

void	echo(t_parser *parser)
{
	float	flg;
	int		i;

	flg = false;
	i = 0;
	while (parser->command->content && parser->command->content && parser->command->content[0] == '-'
			&& parser->command->content[1] == 'n')
	{
		while (parser->command->content[i] == 'n')
			parser->command = parser->command->next;
		if (parser->command->content[i] == '\0')
			flg = true;
		else
			break ;
		i++;
	}
	parser->command = parser->command->next;
	while (parser->command != NULL)
	{
		ft_putstr_fd(parser->command->content, STDOUT_FILENO);
		if (parser->command->next != NULL)
			ft_putchar_fd(' ', STDOUT_FILENO);
		parser->command = parser->command->next;
	}
	if (flg == false)
		ft_putchar_fd('\n', STDOUT_FILENO);
}

// este echo da segfault com dois pipes. Tem de haver uma condicao que diz que so faz o ultimo echo do ultimo pipe e o resto ignora

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
	if (!ft_strncmp(prompt->parser->command->content, "echo", 5))
		echo(prompt->parser);
	else if (!ft_strncmp(prompt->parser->command->content, "pwd", 4))
		pwd();
	else if (!ft_strncmp(prompt->parser->command->content, "env", 4))
		env_builtin(prompt); 
	/*else if (!ft_strncmp(prompt->lexer->content, "cd", 3))
		cd();*/
	/*else if (!ft_strncmp(prompt->lexer->content, "export", 7))
		export();
	else if (!ft_strncmp(prompt->lexer->content, "unset", 6))
		unset();*/
	//else (!ft_strncmp(prompt->lexer->content, "exit", 5))
	//	exit();
}
