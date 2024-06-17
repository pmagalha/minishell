/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-ppe <joao-ppe@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 14:35:47 by joao-ppe          #+#    #+#             */
/*   Updated: 2024/06/17 18:02:00 by joao-ppe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	handle_redirects(t_prompt *prompt, t_parser *parser)
{
	t_lexer		*redir;

	redir = parser->redirects;
	while (redir)
	{
		if (redir->type == REDIR_OUT || redir->type == REDIR2_OUT)
		{
			if (set_fd_out(prompt, redir))
				return (1);
		}
		else if (redir->type == HEREDOC)
		{
			if (set_hdfile_in(parser->hd_file))
				return (1);
		}
		else if (redir->type == REDIR_IN)
		{
			if (set_fd_in(prompt, redir))
				return (1);
		}
		redir = redir->next;
	}
	return (0);
}

int	set_hdfile_in(char *file)
{
	int		fd;

	fd = open(file, O_RDONLY);
	if (dup2(fd, STDIN_FILENO) < 0)
		return (1);
	close(fd);
	return (0);
}

int	set_fd_in(t_prompt *prompt, t_lexer *redir)
{
	int		fd;
	char	*file;

	file = redir->content;
	fd = open(file, O_RDONLY);
	if (fd < 0)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(file, STDERR_FILENO);
		ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
		free_data(prompt);
		exit (1);
	}
	if (dup2(fd, STDIN_FILENO) < 0)
	{
		close(fd);
		return (1);
	}
	close(fd);
	return (0);
}

int	set_fd_out(t_prompt *prompt, t_lexer *redir)
{
	int		fd;
	char	*file;

	file = redir->content;
	fd = 0;
	if (redir->type == REDIR2_OUT)
		fd = open(file, O_CREAT | O_RDWR | O_APPEND, 0644);
	else if (redir->type == REDIR_OUT)
		fd = open(file, O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (fd < 0)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(file, STDERR_FILENO);
		ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
		free_data(prompt);
		exit (1);
	}
	if (dup2(fd, STDOUT_FILENO) < 0)
		return (1);
	return (0);
}

int	check_fd(t_parser *parser, int end[2])
{
	int	fd_in;

	fd_in = end[0];
	if (parser->hd_file)
	{
		close(end[0]);
		fd_in = open(parser->hd_file, O_RDONLY);
	}
	return (fd_in);
}
