/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmagalha <pmagalha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 14:30:29 by joao-ppe          #+#    #+#             */
/*   Updated: 2024/05/07 16:01:37 by pmagalha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_delimiter(t_lexer *redir)
{
	t_lexer	*head;
	char	*delimiter;

	delimiter = NULL;
	head = redir;
	while (head->type != HEREDOC)
		head = head->next;
	if (head->content)
		delimiter = ft_strdup(head->content);
	return (delimiter);
}

int	create_hdfile(t_prompt *prompt, char *file)
{
	int		fd;

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

	new_input = NULL;
	delimiter = get_delimiter(redir);
	input = get_hd_input(prompt);
	while (input && ft_strncmp(input, delimiter, ft_strlen(delimiter) + 1))
	{
		if (input)
			new_input = expander(input, prompt->env_list, NULL);
		ms_free_string(input);
		if (new_input)
			ft_putstr_fd(new_input, fd);
		ft_putchar_fd('\n', fd);
		input = get_hd_input(prompt);
		ms_free_string(new_input);
		if (redir->next)
			redir = redir->next;
	}
	close(fd);
	ms_free_string(input);
	ms_free_string(delimiter);
}
