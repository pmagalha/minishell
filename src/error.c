/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-ppe <joao-ppe@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 13:12:12 by joao-ppe          #+#    #+#             */
/*   Updated: 2024/06/17 18:01:05 by joao-ppe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

extern int	g_code;

void	ms_error(int error)
{
	g_code = error;
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	if (error == 1)
		ft_putstr_fd("memory error: unable to assign memory\n", STDERR_FILENO);
	else if (error == 2)
		ft_putstr_fd("syntax error: unable to locate closing quotation\n",
			STDERR_FILENO);
	else if (error == 3)
		ft_putstr_fd("Parser problem\n", STDERR_FILENO);
	else if (error == 4)
		ft_putstr_fd("Failed to create pipe\n", STDERR_FILENO);
	else if (error == 5)
		ft_putstr_fd("Failed to fork\n", STDERR_FILENO);
	else if (error == 6)
		ft_putstr_fd("outfile: Error\n", STDERR_FILENO);
	else if (error == 7)
		ft_putstr_fd("infile: No such file or directory\n", STDERR_FILENO);
	else if (error == 8)
		ft_putendl_fd("Path does not exist\n", STDERR_FILENO);
}

char	*file_dir_error(char *tmp)
{
	struct stat	st;
	char		*str;

	str = NULL;
	if (stat(tmp, &st) == 0)
	{
		if (S_ISDIR(st.st_mode))
			str = ft_strjoin(tmp, ": Is a directory");
		else if (S_IXUSR)
			str = ft_strjoin(tmp, ": Permission denied");
	}
	else
		str = ft_strjoin(tmp, ": No such file or directory");
	return (str);
}

int	cmd_not_found(t_parser *parser)
{
	struct stat	st;
	char		*str;
	char		*tmp;
	int			status;

	status = 127;
	if (!parser->command || !parser->command->content)
		return (1);
	if (parser->command->content[0])
		tmp = ft_strdup(parser->command->content);
	else
		tmp = ft_strdup("\'\'");
	if ((parser->command->content[0] == '/' || parser->command->content[0]
			== '.') && parser->command->content[1])
		str = file_dir_error(tmp);
	else
		str = ft_strjoin(tmp, ": command not found");
	ft_putendl_fd(str, STDERR_FILENO);
	if ((parser->command->content[0] == '/'
			|| parser->command->content[0] == '.')
		&& stat(tmp, &st) == 0 && (S_ISDIR(st.st_mode) | S_IXUSR))
		status = 126;
	ms_free_string(tmp);
	ms_free_string(str);
	return (status);
}

void	export_error(char *content)
{
	ft_putstr_fd("minishell: export: `", STDERR_FILENO);
	if (content[0] != 32 || (content[0] > 9 && content[0] < 13))
		ft_putstr_fd(content, STDERR_FILENO);
	ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
}

int	ms_cd_error(void)
{
	ft_putstr_fd("minishell: cd: too many arguments\n", STDERR_FILENO);
	return (1);
}
