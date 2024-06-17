/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-ppe <joao-ppe@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 14:30:29 by joao-ppe          #+#    #+#             */
/*   Updated: 2024/06/17 18:01:34 by joao-ppe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

bool	get_delimiter(t_lexer *redir, char **del)
{
	t_lexer	*head;
	bool	quotes;

	head = redir;
	quotes = false;
	while (head->type != HEREDOC)
		head = head->next;
	if ((head->content[0] == '\"'
			&& head->content[ft_strlen(head->content) - 1] == '\"')
		|| (head->content[0] == '\''
			&& head->content[ft_strlen(head->content) - 1] == '\''))
		quotes = true;
	if (head->content && quotes)
		*del = ft_strndup(head->content + 1, ft_strlen(head->content) - 2);
	else if (head->content && !quotes)
		*del = ft_strndup(head->content, ft_strlen(head->content));
	return (quotes);
}

int	create_hdfile(t_prompt *prompt, char *file)
{
	int	fd;

	fd = open(file, O_CREAT | O_RDWR | O_APPEND, 0644);
	if (fd < 0)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(file, STDERR_FILENO);
		ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
		free_data(prompt);
		exit (1);
	}
	return (fd);
}

int	set_heredoc(t_prompt *prompt, t_parser *parser)
{
	int		fd;
	t_lexer	*redir;

	redir = parser->redirects;
	while (redir)
	{
		if (redir->type == HEREDOC)
		{
			if (parser->hd_file)
			{
				unlink(parser->hd_file);
				free(parser->hd_file);
			}
			parser->hd_file = ft_strdup("hd_file.tmp");
			fd = create_hdfile(prompt, parser->hd_file);
			send_heredoc(prompt, redir, fd);
		}
		redir = redir->next;
	}
	return (0);
}

void	send_heredoc(t_prompt *prompt, t_lexer *redir, int fd)
{
	char	*input;
	char	*delimiter;
	char	*new_input;
	bool	quotes;

	new_input = NULL;
	delimiter = NULL;
	quotes = get_delimiter(redir, &delimiter);
	input = get_hd_input(prompt);
	while (input && ft_strncmp(input, delimiter, ft_strlen(delimiter) + 1))
	{
		if (input && !quotes)
			new_input = expander(input, prompt->env_list, NULL);
		else
			new_input = ft_strdup(input);
		ms_free_string(input);
		print_hd(new_input, fd);
		input = get_hd_input(prompt);
		ms_free_string(new_input);
		if (redir->next)
			redir = redir->next;
	}
	close(fd);
	ms_free_string(input);
	ms_free_string(delimiter);
}
