/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-ppe <joao-ppe@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 13:15:12 by pmagalha          #+#    #+#             */
/*   Updated: 2024/06/17 18:01:12 by joao-ppe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

extern int	g_code;

void	execute(t_prompt *prompt)
{
	int			end[2];
	int			fd_in;
	t_parser	*parser;

	fd_in = STDIN_FILENO;
	parser = prompt->parser;
	if (!parser->next)
		return (single_command(prompt, prompt->parser));
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

int	exec_path(t_prompt *prompt, t_parser *parser, char **paths)
{
	int		i;
	char	*path;
	char	**env_array;
	char	**parser_array;

	i = -1;
	if (!paths)
		return (cmd_not_found(parser));
	while (paths[++i])
	{
		path = ft_strjoin(paths[i], parser->command->content);
		if (!access(path, F_OK))
		{
			env_array = convert_env(prompt->env_list);
			parser_array = convert_parser(parser);
			execve(path, parser_array, env_array);
			free (path);
			free_array(env_array);
			free_array(parser_array);
			return (126);
		}
		free (path);
	}
	g_code = cmd_not_found(parser);
	return (g_code);
}

int	handle_command(t_prompt *prompt, t_parser *parser)
{
	char	**paths;

	paths = NULL;
	if (!parser->command && !parser->redirects)
	{
		free_data(prompt);
		exit (0);
	}
	if (parser->redirects)
		g_code = handle_redirects(prompt, parser);
	if (parser->builtin)
		exit_builtin(prompt, parser);
	if (parser->command && parser->command->content)
	{
		if (execute_dpath(prompt, parser) != 0)
			g_code = 126;
		else
		{
			paths = get_paths(prompt);
			g_code = exec_path(prompt, parser, paths);
			free_array(paths);
		}
	}
	free_data(prompt);
	exit (g_code);
}

void	single_command(t_prompt *prompt, t_parser *parser)
{
	int		pid;
	int		status;
	char	*cmd;

	status = 0;
	if (!parser->command)
		cmd = NULL;
	else
		cmd = prompt->parser->command->content;
	if (set_heredoc(prompt, parser))
		return ;
	if (cmd && (!ft_strncmp(cmd, "exit", 5) || !ft_strncmp(cmd, "cd", 3)
			|| !ft_strncmp(cmd, "export", 7) || !ft_strncmp(cmd, "unset", 6)))
	{
		g_code = exec_builtins(prompt, parser);
		return ;
	}
	pid = fork();
	if (pid == 0)
		handle_command(prompt, parser);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		g_code = WEXITSTATUS(status);
}

int	execute_dpath(t_prompt *prompt, t_parser *parser)
{
	struct stat		st;
	char			*path;
	char			**env_array;
	char			**parser_array;

	path = parser->command->content;
	env_array = NULL;
	parser_array = NULL;
	if (!stat(path, &st)
		&& (S_ISDIR(st.st_mode)))
		return (cmd_not_found(parser));
	if (!access(path, F_OK))
	{
		env_array = convert_env(prompt->env_list);
		parser_array = convert_parser(parser);
		execve(path, parser_array, env_array);
		free_array(env_array);
		free_array(parser_array);
		return (126);
	}
	return (0);
}
