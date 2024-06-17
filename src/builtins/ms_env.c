/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-ppe <joao-ppe@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 15:52:09 by pmagalha          #+#    #+#             */
/*   Updated: 2024/06/17 18:00:14 by joao-ppe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	count_lines(char **env)
{
	int	count;

	count = 0;
	while (*env)
	{
		count++;
		env++;
	}
	return (count);
}

void	print_ms_env(t_env_list *temp)
{
	while (temp)
	{
		if (temp->value && *temp->value)
		{
			ft_putstr_fd(temp->key, STDOUT_FILENO);
			ft_putchar_fd('=', STDOUT_FILENO);
			ft_putstr_fd(temp->value, STDOUT_FILENO);
			ft_putchar_fd('\n', STDOUT_FILENO);
			temp = temp->next;
		}
		else if (temp->value && !*temp->value)
		{
			ft_putstr_fd(temp->key, STDOUT_FILENO);
			ft_putstr_fd("=""\n", STDOUT_FILENO);
			temp = temp->next;
		}
		else
			temp = temp->next;
	}
}

void	print_env_list(t_env_list *head)
{
	t_env_list	*current;

	current = head;
	while (current != NULL)
	{
		printf("Key: %s | Value: %s \n", current->key, current->value);
		current = current->next;
	}
}

int	ms_env(t_prompt *prompt, t_parser *parser)
{
	t_env_list	*temp;

	temp = prompt->env_list;
	if (parser->command->next)
	{
		ft_putstr_fd("env: '", STDOUT_FILENO);
		ft_putstr_fd(parser->command->next->content, STDOUT_FILENO);
		ft_putstr_fd("': No such file or directory\n", STDOUT_FILENO);
		return (127);
	}
	print_ms_env(temp);
	return (0);
}

char	*get_env(t_prompt *prompt, char *path)
{
	char		*new_path;
	t_env_list	*temp;

	new_path = NULL;
	temp = prompt->env_list;
	while (temp != NULL)
	{
		if (temp && !ft_strncmp(temp->key, path, ft_strlen(temp->key)))
			new_path = temp->value;
		temp = temp->next;
	}
	return (new_path);
}
