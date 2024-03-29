/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmagalha <pmagalha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 13:21:11 by pmagalha          #+#    #+#             */
/*   Updated: 2024/03/29 13:55:58 by pmagalha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_code;

void	ms_pwd(void)
{
	ft_putstr_fd((getcwd(NULL, 0)), STDOUT_FILENO);
	ft_putchar_fd('\n', STDOUT_FILENO);
}

static void	ft_print(t_lexer *command)
{
	while (command)
	{
		ft_putstr_fd(command->content, STDOUT_FILENO);
		command = command->next;
		if (command)
			ft_putchar_fd(' ', STDOUT_FILENO);
	}
}

int	ms_echo(t_parser *parser)
{
	float	flg;
	int		i;
	t_parser *temp;

	flg = false;
	i = 1;
	temp = parser;
	temp->command = temp->command->next;
	while (temp && temp->command && temp->command->content[0] == '-'
				&& temp->command->content[1] == 'n')
	{
		while (temp->command->content[i] == 'n')
			i++;
		if (temp->command->content[i] == '\0')
			flg = true;
		else
			break ;
		temp->command = temp->command->next;
	}
	ft_print(temp->command);
	if (flg == false)
		ft_putchar_fd('\n', STDOUT_FILENO);
	return (0);
}

void	ms_env(t_prompt *prompt)
{
	t_env_list	*temp;

	temp = prompt->env_list;
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

int	ms_cd(t_prompt *prompt)
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
		return (1); // ja da para substituir isto por ms_exit I GUESS??
	}
	else
		new_path = absolute_path(prompt);
	add_path(prompt);
	return (new_path);
}

void free_array(char **arr)
{
    int i = 0;
    if (arr)
    {
        while (arr[i])
            free(arr[i++]);
        free(arr);
    }
}

static void	exit_code(char **str, t_prompt *prompt)
{
	int	exit_code;

	if (!str[0]) // sem argumentos, exit com 0
		exit_code = 0;
	else if (str[1]) // mais do que um argumento, printa erro
	{
		ft_putstr_fd("-minishell: exit: too many arguments\n", STDERR_FILENO);
		exit_code = 1;
		return ;
	}
	else if (str[0][0] == '#') // (shell exit code) MAYBE NOT NECESSARY IDK
		exit_code = g_code;
	else if (ft_isdigit(str[0][0])) // Checka se eh digito
		exit_code = ft_atoi(str[0]);
	else // se o argumento nao for valido
	{
		ft_putstr_fd("-minishell: exit: numeric argument required\n", STDERR_FILENO);
		exit_code = 2;
	}
	free_array(str);
	free_data(prompt);
	exit(exit_code);
}
// valor maximo de erro eh 256. se o valor ultrapassar isto, tem de voltar atras. tipo se for 256 + 1 vai para 0 I GUESS

int ms_exit(t_parser *parser, t_prompt *prompt)
{
    char **str;
    t_lexer *temp;
    int size;
    int i;

    if (!parser)
    {
        free_parser_list(parser);
        exit(g_code);
    }
    size = 0;
    temp = parser->command->next;
    while (temp)
    {
        size++;
        temp = temp->next;
    }
    str = (char **)malloc((size + 1) * sizeof(char *));
    temp = parser->command->next;
    i = 0;
    while (temp)
    {
        str[i] = ft_strdup(temp->content);
        temp = temp->next;
        i++;
    }
    str[size] = NULL;
    free_lexer_list(temp);
    rl_clear_history();
	ft_putstr_fd("exit\n", STDERR_FILENO);
    exit_code(str, prompt);
    free_array(str);
    return (EXIT_SUCCESS);
}

void	exec_builtins(t_prompt *prompt)
{
	if (!ft_strncmp(prompt->parser->builtin, "echo", 4))
		ms_echo(prompt->parser);
	else if (!ft_strncmp(prompt->parser->builtin, "pwd", 3))
		ms_pwd();
	else if (!ft_strncmp(prompt->parser->builtin, "env", 3))
		ms_env(prompt); 
	else if (!ft_strncmp(prompt->parser->builtin, "cd", 2))
		ms_cd(prompt);
	else if (!ft_strncmp(prompt->parser->builtin, "exit", 4))
		ms_exit(prompt->parser, prompt);
	else if (!ft_strncmp(prompt->parser->builtin, "export", 6))
		ms_export(prompt);
	else if (!ft_strncmp(prompt->parser->builtin, "unset", 5))
		ms_unset(prompt);
}
