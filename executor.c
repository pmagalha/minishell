/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmagalha <pmagalha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 13:15:12 by pmagalha          #+#    #+#             */
/*   Updated: 2024/04/25 13:02:08 by pmagalha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_code;

int	fork_parser(t_prompt *prompt, t_parser *parser, int fd_in, int end[2])
{
	static int	i;

	(void)end[2];
	(void)fd_in;
	if (!i)
		i = 0;
	if (prompt->reset == true)
	{
		i = 0;
		prompt->reset = false;
	}
	prompt->pid[i] = fork();
	if (prompt->pid[i] < 0)
	{
		printf("error with pid\n");
		return (1); // METER AQUI UM ERRO DECENTE
	}
 	if (prompt->pid[i] == 0)
	{
		if (dup_parser(prompt, parser, fd_in, end))
			return (1);
	}
	i++;
	return (0);
}

void	execute(t_prompt *prompt)
{
	int			end[2];
	int			fd_in;
	t_parser	*parser;

	fd_in = STDIN_FILENO;
	parser = prompt->parser;
	if (!parser->next)
		return (single_command(prompt, prompt->parser));
	else
	{
		while (parser)
		{
			if (parser->next)
				pipe(end);
			if (set_heredoc(prompt, parser))
				return ;
			if (fork_parser(prompt, parser, fd_in, end))
				return ;
			close(end[1]);
			if (parser->prev)
				close (fd_in);
			fd_in = check_fd(parser, end);
			parser = parser->next;
		}
		wait_pipe(prompt, prompt->pid);
	}
}

int	exec_path(t_prompt *prompt, t_parser *parser, char **paths) // REDUZIR LINHAS
{
	int		i;
	char	*path;
	char	**env_array;
	char	**parser_array;

	i = -1;
	if (!paths)
        return (1);
	while (paths[++i])
	{
		path = ft_strjoin(paths[i], parser->command->content);
		if (!path)
		{
			perror("Memory allocation error");
			return (1); // handle the error appropriately
		}
		if (!access(path, F_OK))
		{
			env_array = convert_env(prompt->env_list);
			parser_array = convert_parser(prompt, parser);
			execve(path, parser_array, env_array);
			free (path);
			if (env_array)
				free_array(env_array);
			if (parser_array)
				free_array(parser_array);
			return (127);
		}
		free (path);
	}
	g_code = cmd_not_found(prompt, parser);
	return (g_code);
}

int	handle_command(t_prompt *prompt, t_parser *parser) // REDUZIR LINHAS
{
	char	**paths;
	char	*file;

	paths = NULL;
	file = NULL;
	if (!parser->command && parser->redirects)
	{
		free_data(prompt);
		exit (0);
	}
 	if (parser->redirects)
		g_code = handle_redirects(prompt, parser);
	if (parser->builtin)
	{
		g_code = exec_builtins(prompt, parser);
		free_data(prompt);
		exit (g_code);
	}
	if (parser->command && parser->command->content)
	{
		paths = get_paths(prompt);
		g_code = exec_path(prompt, parser, paths);
		free_array(paths);
		free_data(prompt);
		exit (g_code);
	}
	exit (g_code);
}

// TRATAR DOS ERROS DA FUNCAO ABAIXO!!! IMPORTANTE
void    single_command(t_prompt *prompt, t_parser *parser)
{
	int		pid;
    int		status;
    char	*command;
	
	status = 0;
	if (!prompt->parser->command)
		return ;
	command = prompt->parser->command->content;
	if (set_heredoc(prompt, parser))
				return ;
	if (command && (!ft_strncmp(command, "exit", 5) || !ft_strncmp(command, "cd", 3)
			|| !ft_strncmp(command, "export", 7) || !ft_strncmp(command, "unset", 6)))
	{
		g_code = exec_builtins(prompt, parser);
		return ;
	}
	pid = fork();
	command = parser->command->content;
	if (pid < 0)
		exit (1); // OU WTV ERRO QUE SEJA
	if (pid == 0)
		handle_command(prompt, parser);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		g_code = WEXITSTATUS(status);
}
