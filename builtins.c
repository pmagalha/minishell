/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-ppe <joao-ppe@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 13:21:11 by pmagalha          #+#    #+#             */
/*   Updated: 2024/04/24 18:58:54 by joao-ppe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_code;

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

static void	ft_print(t_lexer *command)
{
	t_lexer	*temp;

	temp = command;
	while (temp)
	{
		ft_putstr_fd(temp->content, STDOUT_FILENO);
		temp = temp->next;
		if (temp)
			ft_putchar_fd(' ', STDOUT_FILENO);
	}
}

int	ms_echo(t_parser *parser)
{
	int		i;
	float	flg;
	t_lexer	*temp;

	i = 1;
	flg = false;
	temp = parser->command->next;
	while (temp && temp->content[0] == '-' && temp->content[1] == 'n')
	{
		while (temp->content[i] == 'n')
			i++;
		if (temp->content[i] == '\0')
			flg = true;
		else
			break ;
		temp = temp->next;
	}
	ft_print(temp);
	if (flg == false)
	{
		ft_putchar_fd('\n', STDOUT_FILENO);
	}
	return (0);
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
		new_path = change_path(prompt, "OLDPWD");
	else if (temp->next != NULL)
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", STDERR_FILENO);
		return (1);
	}
	else
		new_path = absolute_path(parser);
	if (new_path < 0)
		return (1);
	add_path(prompt);
	return (new_path);
}

int	str_is_digit(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

static void	exit_code(char **str)
{
	int		exit_code;

	exit_code = 0;
	if (!str[0])
		exit_code = 0;
	else if (str[1])
	{
		ft_putstr_fd("-minishell: exit: too many arguments\n", STDERR_FILENO);
		exit_code = 1;
		free_array(str);
		exit(exit_code);
	}
	else if (str_is_digit(str[0]) || ft_atoi(str[0]))
		exit_code = ft_atoi(str[0]);
	else if (str[0][0] && str[0][0] == '-' && str_is_digit(str[0] + 1))
		exit_code = 256 - ft_atoi(str[0] + 1);
	else
	{
		ft_putstr_fd("minishell: exit: numeric ", STDERR_FILENO);
		ft_putstr_fd("argument required\n", STDERR_FILENO);
		exit_code = 2;
	}
	free_array(str);
	exit(exit_code);
}

void	ms_exit_success(t_prompt *prompt, char **str)
{
	rl_clear_history();
	ft_putstr_fd("exit\n", STDOUT_FILENO);
	free_data(prompt);
	exit_code(str);
	free_array(str);
}

int	ms_exit(t_parser *parser, t_prompt *prompt)
{
	int		i;
	int		size;
	char	**str;
	t_lexer	*temp;

	size = 0;
	temp = parser->command->next;
	if (!parser)
	{
		free_data(prompt);
		exit(g_code);
	}
	while (temp && ++size)
		temp = temp->next;
	str = (char **)ft_calloc(size + 1, sizeof(char *));
	temp = parser->command->next;
	i = 0;
	while (temp)
	{
		str[i] = ft_strdup(temp->content);
		temp = temp->next;
		i++;
	}
	ms_exit_success(prompt, str);
	return (EXIT_SUCCESS);
}

int	exec_builtins(t_prompt *prompt, t_parser *parser)
{
	int	status;

	status = 1;
	if (!ft_strncmp(parser->command->content, "echo", 5))
		status = ms_echo(parser);
	else if (!ft_strncmp(parser->command->content, "pwd", 4))
		status = ms_pwd();
	else if (!ft_strncmp(parser->command->content, "env", 4))
		status = ms_env(prompt, parser);
	else if (!ft_strncmp(parser->command->content, "cd", 3))
		status = ms_cd(prompt, parser);
	else if (!ft_strncmp(parser->command->content, "exit", 5))
		return (ms_exit(parser, prompt));
	else if (!ft_strncmp(parser->command->content, "export", 7))
		status = ms_export(prompt, parser);
	else if (!ft_strncmp(parser->command->content, "unset", 6))
		status = ms_unset(prompt, parser);
	return (status);
}
