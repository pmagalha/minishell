/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmagalha <pmagalha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 13:21:11 by pmagalha          #+#    #+#             */
/*   Updated: 2024/04/05 13:52:19 by pmagalha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_code;

void	ms_pwd(void)
{
	char *temp;
	
	temp = getcwd(NULL, 0);
	
	ft_putstr_fd(temp, STDOUT_FILENO);
	ft_putchar_fd('\n', STDOUT_FILENO);
	if (temp)
		free(temp);
}

static void ft_print(t_lexer *command)
{
	t_lexer *temp = command;

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
	int		i = 1;
	float	flg = false;
	t_lexer *temp = parser->command->next; // salta o primeiro command
	while (temp && temp->content[0] == '-' && temp->content[1] == 'n')
	{
		while (temp->content[i] == 'n')
			i++;
		if (temp->content[i] == '\0')
			flg = true;
		else
			break;
		temp = temp->next;
	}
	ft_print(temp); // Printa os commands depois do echo
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

// tratar de ~/..
// tratar do caso de ser um PATH maior do que permitido (checkar path max)
// tratar de cd ../relative_directory

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

void	free_array(char **arr)
{
	int	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

static void	exit_code(char **str/* , t_prompt *prompt */)
{
	int	exit_code;

	//printf("A STRING[0] eh: %s\n", str[0]);
	if (!str[0]) // sem argumentos, exit com 0
		exit_code = 0;
	else if (str[1]) // mais do que um argumento, printa erro
	{
		ft_putstr_fd("-minishell: exit: too many arguments\n", STDERR_FILENO);
		exit_code = 1;
		free_array(str);
		exit(exit_code);
	}
	else if (ft_isdigit(str[0][0])) // Checka se eh digito
		exit_code = ft_atoi(str[0]);
	else // se o argumento nao for valido
	{
		ft_putstr_fd("-minishell: exit: numeric argument required\n", STDERR_FILENO);
		exit_code = 2;
	}
	free_array(str);
	//free_data(prompt);
	//print_parser(prompt);
	//dev_mod(prompt);
	//printf("SAIU AQUI\n");
	exit(exit_code);
}
// valor maximo de erro eh 256. se o valor ultrapassar isto, tem de voltar atras. tipo se for 256 + 1 vai para 0 I GUESS

int ms_exit(t_parser *parser, t_prompt *prompt, t_lexer *start)
{
    char **str;
    t_lexer *temp;
    int size;
    int i;

	//printf("NODE DO LEXER: [%s]\n", prompt->lexer->content);
    if (!parser)
    {
        free_data(prompt);
        exit(g_code);
    }
    size = 0;
    temp = parser->command->next;
	//printf("COMMAND CONTENT: [%s]\n", parser->command->content);
	//printf("COMMAND CONTENT: [%s]\n", parser->command->next->content);
    while (temp)
    {
        size++;
        temp = temp->next;
    }
    str = (char **)ft_calloc(size + 1, sizeof(char *));
	temp = start->next;
    i = 0;
    while (temp)
    {
		str[i] = ft_strdup(temp->content);
		temp = temp->next;
		i++;		
    }
	//printf("22222 A STRING[0] eh: %s\n", str[0]);
	free_lexer_list(&start);
/* 	for (int i = 0 ; str[i] != NULL; i++)
		printf("STRING[i]: %s\n", str[i]); */
	free_data(prompt);
    rl_clear_history();
	ft_putstr_fd("exit\n", STDERR_FILENO);
    //printf("ISTO EH A STRING ANTES: [%s]\n", str[0]);
    exit_code(str);
	free_array(str);
	//printf("ISTO EH A STRING DEPOIS: [%s]\n", str[0]);
    return (EXIT_SUCCESS);
}

void	exec_builtins(t_prompt *prompt, t_lexer *start)
{
	//printf("NODE DO LEXER: [%s]\n", prompt->lexer->content);
	if (!ft_strncmp(prompt->parser->builtin, "echo", 4))
		ms_echo(prompt->parser);
	else if (!ft_strncmp(prompt->parser->builtin, "pwd", 3))
		ms_pwd();
	else if (!ft_strncmp(prompt->parser->builtin, "env", 3))
		ms_env(prompt); 
	else if (!ft_strncmp(prompt->parser->builtin, "cd", 2))
		ms_cd(prompt);
		
	else if (!ft_strncmp(prompt->parser->builtin, "exit", 4))
		ms_exit(prompt->parser, prompt, start);
	else if (!ft_strncmp(prompt->parser->builtin, "export", 6))
		ms_export(prompt);
	else if (!ft_strncmp(prompt->parser->builtin, "unset", 5))
		ms_unset(prompt);
}
