/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmagalha <pmagalha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 14:30:29 by joao-ppe          #+#    #+#             */
/*   Updated: 2024/04/25 12:50:16 by pmagalha         ###   ########.fr       */
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

char	*get_hdfile(t_parser *parser, t_lexer *redir)
{
	t_lexer	*head;
	char	*file;

	(void)parser;
	head = redir;
	file = NULL;
	if (!head->content)
		return (file);
	while (head && (ft_strncmp(head->content, ">", 2) && ft_strncmp(head->content, ">>", 3)))
		head = head->next;
	if (head->content && head->next->content)
		file = ft_strdup(head->next->content);
	return (file);
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
	input = readline("> ");
	while (ft_strncmp(input, delimiter, ft_strlen(delimiter) + 1))
	{
		if (input)
			new_input = expander(input, prompt->env_list);
		ms_free_string(input);
		ft_putstr_fd(new_input, fd);
		ft_putchar_fd('\n', fd);
		input = readline("> ");
		ms_free_string(new_input);
		if (redir->next)
			redir = redir->next;
	}
	close(fd);
	ms_free_string(input);
	ms_free_string(delimiter);
}
