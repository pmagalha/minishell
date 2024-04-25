/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils1.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmagalha <pmagalha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 14:26:51 by joao-ppe          #+#    #+#             */
/*   Updated: 2024/04/25 17:44:55 by pmagalha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	dup_parser(t_prompt *prompt, t_parser *parser, int fd_in, int end[2])
{
	(void)prompt;
	if (parser->prev && dup2(fd_in, STDIN_FILENO) < 0)
	{
		return (1);
	}
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

char	**get_paths(t_prompt *prompt) // REDUZIR LINHAS
{
	char	*path;
	char	*tmp;
	char	**paths;
	int		i;

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
			while (--i >= 0)
				ms_free_string(paths[i]);
			free_array(paths);
			return (NULL);
		}
		paths[i] = tmp;
	}
	return (paths);
}

