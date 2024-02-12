/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 13:21:11 by pmagalha          #+#    #+#             */
/*   Updated: 2024/02/08 19:13:34 by marvin           ###   ########.fr       */
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
	if (!ft_strncmp(temp->content, "-n", ft_strlen(temp->content)))
	{
		suppress_newline = true;  // mete a flag para dar suppress da newline 
		temp = temp->next;
	}
	while (temp != NULL)
	{
			ft_putstr_fd(temp->content, STDOUT_FILENO);
			if (temp->next != NULL)
				ft_putchar_fd(' ', STDOUT_FILENO);
			temp = temp->next;
	}
	if (!suppress_newline)
		ft_putchar_fd('\n', STDOUT_FILENO);
	free(temp);
} // o -n eh so no inicio depois do echo, e eu enganei me e meti a funcionar em toda a string

void	env_builtin(t_prompt *prompt)
{
	t_env_list	*temp;

	temp = prompt->env_list;
	while (temp && temp->key)
	{
		ft_putstr_fd(temp->key, STDOUT_FILENO);
		ft_putchar_fd('=', STDOUT_FILENO);
		ft_putstr_fd(temp->value, STDOUT_FILENO);
		ft_putchar_fd('\n', STDOUT_FILENO);
		temp = temp->next;
	}
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

static void	add_path(t_prompt *prompt)
{
	char		*tmp;
	char		*oldpath;
	t_env_list	*env;

	oldpath = get_env(prompt, "PWD");
	env = prompt->env_list;
	while (env)
	{
		if (!ft_strncmp(env->key, "PWD", 4))
		{
			tmp = getcwd(NULL, 0);
			env->value = ft_strdup(tmp);
			free(tmp);
		}
		if (!ft_strncmp(env->key, "OLDPWD", 7))
			env->value = ft_strdup(oldpath);
		env = env->next;
	}
	free(oldpath);
}

static int	absolute_path(t_prompt *prompt)
{
	int	new_path;

	new_path = chdir(prompt->parser->command->next->content); // this works for .. and for cd without anything because the chdir function can receive those
	return (new_path);
}

int	cd(t_prompt *prompt)
{
	int		new_path;
	t_lexer	*temp;

	temp = prompt->parser->command->next;
	if (!temp || !temp->content[0] || (temp->content[0] &&
			(!ft_strncmp(temp->content, "~", 2) || (temp->content[0] == '-' && (temp->content[1] == '-' && !temp->content[2])))))
		new_path = change_path(prompt, "HOME");
	else if (temp->content[0] == '-' && temp->content[1] != '-')
		new_path = change_path(prompt, "OLDPWD");
	else if (temp->next != NULL)
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", STDERR_FILENO);
		return (1);
	}
	else
		new_path = absolute_path(prompt);
	add_path(prompt);
	return (new_path);
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
