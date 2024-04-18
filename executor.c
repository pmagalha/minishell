/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmagalha <pmagalha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 13:15:12 by pmagalha          #+#    #+#             */
/*   Updated: 2024/04/18 16:23:07 by pmagalha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_code;

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

void print_pid_array(const t_prompt *prompt) 
{
    if (prompt == NULL || prompt->pid == NULL) 
	{
        printf("PID array is NULL\n");
        return;
    }
    printf("PID Array:\n");
    for (int i = 0; i < prompt->pid_size; i++) 
	{
        printf("[%d]: %d\n", i, prompt->pid[i]);
    }
}

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
/* 	if (i >= prompt->pid_size) {
        printf("Error: pid array out of bounds\n");
        return (1);
    } */
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

void	wait_pipe(t_prompt *prompt, int *pid)
{
	t_parser 	*parser;
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

void	execute(t_prompt *prompt)
{
	int			end[2];
	int			fd_in;
	t_parser	*parser;

	fd_in = STDIN_FILENO;
	parser = prompt->parser;
/*   	printf("PID ARRAY BEFORE\n");
	print_pid_array(prompt);
	printf("\n"); */
	if (!parser->next)
		return (single_command(prompt, prompt->parser));
	else
	{
		while (parser)
		{
			//ft_putstr_fd("ENTREI\n", STDOUT_FILENO);
			if (parser->next)
				pipe(end);
			/* if (dup_parser(prompt, parser, fd_in, end) < 0)
				return ; */
			if (fork_parser(prompt, parser, fd_in, end))
				return ;
			//printf("Execute vai dar close do end[1]: %d com pid[%d]\n", end[1], getpid());
			close(end[1]);
 			//printf("Execute vai dar close do fd_in: %d com pid[%d]\n", fd_in, getpid());
			if (parser->prev)
				close (fd_in);
			fd_in = end[0];
			parser = parser->next;
		}
/*   		printf("PID ARRAY AFTER\n");
		print_pid_array(prompt); */
		wait_pipe(prompt, prompt->pid);
	}
}

char	**get_paths(t_prompt *prompt)
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

static int	lexer_list_size(t_lexer *lexer)
{
	int count = 0;
	t_lexer *current;

	current = lexer;
	while (current != NULL)
	{
		count++;
		current = current->next;
	}
	return (count);
}


static int	env_list_size(t_env_list *env_list)
{
	int count = 0;
	t_env_list *current = env_list;

	while (current != NULL)
	{
		count++;
		current = current->next;
	}
	return (count);
}

char	**convert_env(t_env_list *env_list)
{
	int			i;
	int			size;
	char 		**env_array;
	t_env_list *head;
	char 		*temp;

	i = 0;
	temp = NULL;
	env_array = NULL;
	head = env_list;
	size = env_list_size(env_list);
	env_array = (char **)ft_calloc(size + 1, sizeof(char *));
	if (!env_array)
		return (NULL);
 	while (head)
	{
		temp = ms_safejoin(head->key, NULL);
		temp = ms_safejoin(temp, ft_strdup("="));
		if (head->value && *head->value)
			temp = ms_safejoin(temp, head->value);
		env_array[i] = malloc((ft_strlen(temp) + 1) * sizeof(char));
		if (!env_array[i])
		{
			free_array(env_array);
			return (NULL);
		}
		ft_strlcpy(env_array[i], temp, ft_strlen(temp) + 1); 
		ms_free_string(temp);
		head = head->next;
		i++;
	}
	return (env_array);
}



char	**convert_parser(t_prompt *prompt, t_parser *parser)
{
	char	**parser_array;
	size_t	size;
	t_lexer	*head;
	int		i;
	
	i = 0;
	size = lexer_list_size(parser->command);
	head = prompt->parser->command;
	parser_array = (char **)ft_calloc(size + 1, sizeof(char *));
	if (!parser_array)
		return (NULL);
	while (head)
	{
		parser_array[i] = ft_strdup(head->content);
		head = head->next;
		i++;
	}
	return (parser_array);
}

static char	*file_dir_error(char *tmp)
{
	struct stat	st;
	char		*str;

	if (stat(tmp, &st) == 0)
	{
		if (S_ISDIR(st.st_mode))
			str = ft_strjoin(tmp, ": Is a directory");
		else if (S_IXUSR)
			str = ft_strjoin(tmp, ": Permission denied");
	}
	else
		str = ft_strjoin(tmp, ": No such file or directory\n");
	return (str);
}

int	cmd_not_found(t_prompt *prompt, t_parser *parser)
{
	struct stat	st;
	char		*str;
	char		*tmp;
	int			status;

	status = 127;
	(void)prompt;
	if (!parser->command && !parser->command)
		return (1);
	if (parser->command->content[0])
		tmp = ft_strdup(parser->command->content);
	else
		tmp = ft_strdup("\'\'");
	if ((parser->command->content[0] == '/' || parser->command->content[0] == '.') && parser->command->content[1])
	{
		str = file_dir_error(tmp);
	}
	else
		str = ft_strjoin(tmp, ": command not found");
	ft_putendl_fd(str, STDERR_FILENO);
	if ((parser->command->content[0] == '/' || parser->command->content[0] == '.')
		&& stat(tmp, &st) == 0 && (S_ISDIR(st.st_mode) | S_IXUSR))
		status = 126;
	free(tmp);
	free(str);
	return (status);
}

int	exec_path(t_prompt *prompt, t_parser *parser, char **paths)
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
			parser_array = convert_parser(prompt, prompt->parser);
			execve(path, parser_array, env_array);
			free(path);
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

int	handle_command(t_prompt *prompt, t_parser *parser)
{
	char	**paths;
	char	*file;

	paths = NULL;
	file = NULL;
/* 	printf("content is [%s] :\n" ,parser->command->content);
	printf("content is [%s] :\n" ,parser->command->next->content); */
	if (!parser->command && parser->redirects)
	{
		printf("ERROR: SHEET\n");
		return (1);
	}
 	if (parser->redirects)
	{
		g_code = handle_redirects(prompt);
	}
	//printf("CONTENT: [%s]\n", parser->command->content);
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
		printf("sai pela handle command\n");
		exit (g_code);
	}
/* 	free_array(paths);
	free_data(prompt); // added this here */
/* 	if (cmd_not_found(prompt, parser))
	{
		free_data(prompt);
		exit(127);
	} */
	exit (g_code);
}

void	set_heredoc(t_prompt *prompt)
{
	int		fd;
	char	*input;
	char	*delimiter;
	char	*new_input;
	char	*file;

	new_input = NULL;
	prompt->parser->hd_file = get_hdfile(prompt->parser->redirects);
	file = prompt->parser->hd_file;
	delimiter = get_delimiter(prompt->parser);
	fd = create_temp_file(new_input, file);
	while (ft_strncmp(input, delimiter, ft_strlen(delimiter) + 1))
	{
		ms_free_string(input);
		input = readline("> ");
		if (input)
			new_input = expander(input, prompt->env_list);
		ft_putstr_fd(new_input, fd);
	}
	ms_free_string(input);
	close(fd);
	ms_free_string(delimiter);
}

char	*get_delimiter(t_parser *parser)
{
	t_lexer	*head;
	char	*delimiter;

	delimiter = NULL;
	head = parser->redirects;
	while (ft_strncmp(head->content, "<<", 3))
		head = head->next;
	if (head->content)
		delimiter = ft_strdup(head->content);
	return (delimiter);
	
}

int	create_temp_file(char *input, char *file)
{
	int		fd;

	fd = 0;
	if (!file)
		file = ft_strdup("temp");
	if (input)
	{
		fd = open(file, O_CREAT | O_RDWR | O_APPEND, 0644);
		if (fd < 0)
			return (1);
	}
	ms_free_string(file);
	return (fd);
		
}

char	*get_hdfile(t_lexer *redir)
{
	t_lexer	*head;
	char	*file;

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

int	handle_redirects(t_prompt *prompt)
{
	t_lexer		*redir;

	redir = prompt->parser->redirects;
	while (redir)
	{
		if (redir->type == REDIR_OUT || redir->type == REDIR2_OUT) // > / >>
		{
			if (set_fd_out(redir))
				return (1);
		}
		else if (redir->type == REDIR_IN) // <
		{
			if (set_fd_in(redir))
				return (1);
		}
		else if (redir->type == HEREDOC) // <<
		{
			set_heredoc(prompt);
			return (1);
		}
		redir = redir->next;
	}
	return (0);
}
int	set_fd_in(t_lexer *redir)
{
	int		fd;
	char	*file;

	file = redir->content;
	fd = open(file, O_RDONLY);
	if (dup2(fd, STDIN_FILENO) < 0)
		return (1);
	close(fd);
	return (0);
}

int	set_fd_out(t_lexer *redir)
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
		return (1);
	if (dup2(fd, STDOUT_FILENO) < 0)
		return (1);
	close(fd);
	return (0);
}

// TRATAR DOS ERROS DA FUNCAO ABAIXO!!! IMPORTANTE CRL

void    single_command(t_prompt *prompt, t_parser *parser)
{
	int		pid;
    int		status;
    char	*command;
	
	status = 0;
	if (!prompt->parser->command)
		return ;
	command = prompt->parser->command->content;
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
	//printf("PIDE: [%d]\n", pid);
	if (WIFEXITED(status))
		g_code = WEXITSTATUS(status);
}
