/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 13:21:11 by pmagalha          #+#    #+#             */
/*   Updated: 2024/02/07 17:33:22 by marvin           ###   ########.fr       */
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
	char	*new_path;
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
	int	decree;
	
	temp = get_env(prompt, path);
	// colocar aqui cenas para !temp
	decree = chdir(temp);
	//free (temp);
	if (decree != 0)
	{
		ft_putstr_fd("cd: ", STDERR_FILENO);
		ft_putstr_fd(path, STDERR_FILENO);
		ft_putstr_fd(" No such file or directory\n", STDERR_FILENO);
	}
	return (decree);
}

static void	add_path(t_prompt *prompt)
{
	t_env_list *env;
	char	*tmp;
	char	*oldpath;

	oldpath = get_env(prompt, "PWD");
	env = prompt->env_list;
	while (env)
	{
		if (!ft_strncmp(env->key, "PWD", 4))
		{
			//if (env->value)
				//free(env->value);
			tmp = getcwd(NULL, 0);
			env->value = ft_strdup(tmp);
			free(tmp);
		}
		else if (!ft_strncmp(env->key, "OLDPWD", 7))
		{
			if (env->value)
				free(env->value);
			env->value = ft_strdup(oldpath);
		}
		env = env->next;
	}
	free(oldpath);
}

int	cd(t_prompt *prompt)
{
	int	decree;
	t_lexer	*temp;

	temp = prompt->parser->command->next;
	if (!temp || !temp->content[0] || (temp->content[0]
			&& !ft_strncmp(temp->content, "~", 2) && !temp->content[1]))
		decree = change_path(prompt, "HOME");
	else if (!ft_strncmp(temp->content, "-", 1))
		decree = change_path(prompt, "OLDPWD");
	else if (!ft_strncmp(temp->content, "..", 3))
        decree = chdir("..");
	else if (temp->next != NULL)
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", STDERR_FILENO);
		return (1);
	}
	add_path(prompt);
	return (decree);
} // falta tratar de diretorios absolutos e de --

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
