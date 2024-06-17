/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils1.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-ppe <joao-ppe@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 14:26:51 by joao-ppe          #+#    #+#             */
/*   Updated: 2024/06/17 18:01:07 by joao-ppe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	dup_parser(t_prompt *prompt, t_parser *parser, int fd_in, int end[2])
{
	if (parser->prev && dup2(fd_in, STDIN_FILENO) < 0)
		return (1);
	close(end[0]);
	if (parser->next && dup2(end[1], STDOUT_FILENO) < 0)
		return (1);
	close(end[1]);
	if (parser->prev)
		close(fd_in);
	handle_command(prompt, parser);
	return (0);
}

void	wait_pipe(t_prompt *prompt, int *pid)
{
	t_parser	*parser;
	int			n_pipes;
	int			i;
	int			status;

	parser = prompt->parser;
	n_pipes = 0;
	n_pipes = prompt->pid_size;
	i = -1;
	while (++i < n_pipes - 1)
		waitpid(pid[i], &status, 0);
	waitpid(pid[i], &status, 0);
	if (WIFEXITED(status))
		g_code = WEXITSTATUS(status);
}

void	free_paths(char **paths)
{
	int	i;

	i = -1;
	while (paths[++i])
		ms_free_string(paths[i]);
	free_array(paths);
}

char	**get_paths(t_prompt *prompt)
{
	int		i;
	char	*tmp;
	char	*path;
	char	**paths;

	path = find_value("PATH", prompt->env_list);
	if (!path)
		return (NULL);
	paths = ft_split(path, ':');
	free(path);
	if (!paths)
		return (NULL);
	i = -1;
	while (paths[++i])
	{
		tmp = ft_strjoin(paths[i], "/");
		free(paths[i]);
		if (!tmp)
		{
			free_paths(paths);
			return (NULL);
		}
		paths[i] = tmp;
	}
	return (paths);
}

int	fork_parser(t_prompt *prompt, t_parser *parser, int fd_in, int end[2])
{
	static int	i;

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
		ft_putstr_fd("Unable to generate PID\n", STDERR_FILENO);
		return (1);
	}
	if (prompt->pid[i] == 0)
	{
		if (dup_parser(prompt, parser, fd_in, end))
			return (1);
	}
	i++;
	return (0);
}
