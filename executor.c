/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-ppe <joao-ppe@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 13:15:12 by pmagalha          #+#    #+#             */
/*   Updated: 2024/04/10 18:31:46 by joao-ppe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_code;

void	execute(t_prompt *prompt)
{
	t_parser	*parser;
	
	parser = prompt->parser;
	if (!parser->next)
		single_command(prompt, prompt->parser);
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
	i = -1;
	while (paths[++i])
	{
		tmp = ft_strjoin(paths[i], "/");
		free(paths[i]);
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

/* char	*get_full_string(t_env_list *env_list)
{
	int		i;
	int		len;
	char	*full_string;

	i = 0;
	len = ft_strlen(env);
	full_string = malloc(sizeof(char) * len + 1);
	if (!full_string)
		return (NULL);
	ft_strlcpy(full_string, env, len + 1);
	
	return (full_string);
} 
 */

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

int	exec_path(t_prompt *prompt, char **paths)
{
	int		i;
	char	*path;
	char	**env_array;
	char	**parser_array;

	i = -1;
	path = NULL;
	if (!paths)
		return (1);
	while (paths[++i])
	{
		path = ft_strjoin(paths[i], prompt->parser->command->content);
		if (!access(path, F_OK) && prompt->parser->command->content[1])
		{
			env_array = convert_env(prompt->env_list);
			parser_array = convert_parser(prompt, prompt->parser);
			execve(path, parser_array, env_array);
			free(path);
			free_array(env_array);
			free_array(parser_array);
			g_code = 127;
			return (g_code);
		}
		free (path);
	}
	return (0);
}

int	handle_command(t_prompt *prompt, t_parser *parser)
{
	char	**paths;

	paths = NULL;
	if (!parser->command && parser->redirects)
	{
		printf("ERROR: SHEET\n");
		return (1);
	}
 	else if (parser->redirects)
		g_code = handle_redirects(prompt);
	if (parser->builtin)
	{
		g_code = exec_builtins(prompt, parser);
		free_data(prompt);
		exit (g_code);
	}
	paths = get_paths(prompt);
	g_code = exec_path(prompt, paths);
	free_array(paths);
	if (cmd_not_found(prompt, parser))
	{
		free_data(prompt);
		exit (127);
	}
	return (cmd_not_found(prompt, parser));
}

// tem que se verificar o caso de ter um redirect sem command. Ex: [>> oi] assim cria ficheiro 'oi' / [>>] Assim da erro / [<< oi] [<] Assim da erro

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
	{
		return (1);
	}
	if (dup2(fd, STDOUT_FILENO) < 0) {
		return (1);
	}
	close(fd);
	return (0);
}

// TRATAR DOS ERROS DA FUNCAO ABAIXO!!! IMPORTANTE CRL
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
			;
		}
		redir = redir->next;
	}
	return (0);
}

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
