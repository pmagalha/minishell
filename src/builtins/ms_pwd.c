/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-ppe <joao-ppe@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 16:04:45 by pmagalha          #+#    #+#             */
/*   Updated: 2024/06/17 18:00:19 by joao-ppe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ms_pwd(void)
{
	char	*temp;

	temp = getcwd(NULL, 0);
	ft_putstr_fd(temp, STDOUT_FILENO);
	ft_putchar_fd('\n', STDOUT_FILENO);
	if (temp)
		free(temp);
	return (0);
}

void	create_pwd(t_prompt *prompt)
{
	char		*key;
	char		*new_pwd;
	t_env_list	*head;
	t_env_list	*new_node;

	key = NULL;
	head = prompt->env_list;
	new_pwd = getcwd(NULL, 0);
	if (get_env(prompt, "PWD"))
	{
		while (ft_strncmp(head->key, "PWD", 4))
			head = head->next;
		ms_free_string(head->value);
		head->value = new_pwd;
	}
	else
	{
		key = ft_strdup("PWD");
		new_node = create_key_value(key, new_pwd);
		token_add_back_env(&(prompt->env_list), new_node);
		ms_free_string(key);
		ms_free_string(new_pwd);
	}
}
