/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-ppe <joao-ppe@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 16:02:44 by pmagalha          #+#    #+#             */
/*   Updated: 2024/06/17 17:59:20 by joao-ppe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	change_path(t_prompt *prompt, char *path)
{
	char	*temp;
	int		new_dir;

	temp = get_env(prompt, path);
	if (!temp)
	{
		ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
		ft_putstr_fd(path, STDERR_FILENO);
		ft_putstr_fd(" not set\n", STDERR_FILENO);
		return (1);
	}
	new_dir = chdir(temp);
	if (new_dir != 0)
	{
		ft_putstr_fd("cd: ", STDERR_FILENO);
		ft_putstr_fd(path, STDERR_FILENO);
		ft_putstr_fd(" No such file or directory\n", STDERR_FILENO);
	}
	return (new_dir);
}

static void	add_path_aux(t_env_list *env, char *old_path, char *old_value)
{
	char		*tmp;

	while (env != NULL)
	{
		if (!ft_strncmp(env->key, "PWD", 4))
		{
			old_value = env->value;
			tmp = getcwd(NULL, 0);
			env->value = ft_strdup(tmp);
			if (tmp)
				free (tmp);
		}
		else if (!ft_strncmp(env->key, "OLDPWD", 7))
		{
			old_value = env->value;
			env->value = ft_strdup(old_path);
			if (old_value)
				free (old_value);
		}
		env = env->next;
	}
}

static void	add_path(t_prompt *prompt)
{
	char		*old_value;
	char		*old_path;
	t_env_list	*env;

	old_value = NULL;
	old_path = get_env(prompt, "PWD");
	env = prompt->env_list;
	add_path_aux(env, old_path, old_value);
	free (old_path);
}

static int	absolute_path(t_parser *parser)
{
	int	new_path;

	new_path = chdir(parser->command->next->content);
	if (new_path != 0)
	{
		ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
		ft_putstr_fd(parser->command->next->content, STDERR_FILENO);
		perror(" ");
	}
	return (new_path);
}

int	ms_cd(t_prompt *prompt, t_parser *parser)
{
	int		new_path;
	t_lexer	*temp;

	temp = parser->command->next;
	new_path = 0;
	if (!temp || !temp->content[0] || (temp->content[0]
			&& (!ft_strncmp(temp->content, "~", 2) || (temp->content[0] == '-'
					&& (temp->content[1] == '-' && !temp->content[2])))))
		new_path = change_path(prompt, "HOME");
	else if (temp->content[0] == '-' && temp->content[1] != '-')
	{
		if (temp->next)
			return (ms_cd_error());
		new_path = change_path(prompt, "OLDPWD");
	}
	else if (temp->next != NULL)
		return (ms_cd_error());
	else
		new_path = absolute_path(parser);
	if (new_path < 0)
		return (1);
	add_path(prompt);
	return (new_path);
}
