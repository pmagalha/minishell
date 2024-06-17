/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-ppe <joao-ppe@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 16:03:55 by pmagalha          #+#    #+#             */
/*   Updated: 2024/06/17 18:00:17 by joao-ppe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

extern int	g_code;

int	str_is_digit(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

static int	exit_code(char **str, t_prompt *prompt)
{
	int		exit_code;

	exit_code = 0;
	if (!str[0])
		exit_code = 0;
	else if (str_is_digit(str[0]) && str[1])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
		exit_code = 1;
		return (1);
	}
	else if (str_is_digit(str[0]) && ft_atoi(str[0]))
		exit_code = ft_atoi(str[0]);
	else if (str[0][0] && str[0][0] == '-' && str_is_digit(str[0] + 1))
		exit_code = 256 - ft_atoi(str[0] + 1);
	else
	{
		ft_putstr_fd("minishell: exit: numeric ", STDERR_FILENO);
		ft_putstr_fd("argument required\n", STDERR_FILENO);
		exit_code = 2;
	}
	free_array(str);
	free_data(prompt);
	exit(exit_code);
}

int	ms_exit_success(t_prompt *prompt, char **str)
{
	rl_clear_history();
	ft_putstr_fd("exit\n", STDOUT_FILENO);
	if (exit_code(str, prompt))
	{
		free_array(str);
		return (1);
	}
	free_data(prompt);
	free_array(str);
	return (0);
}

int	ms_exit(t_parser *parser, t_prompt *prompt)
{
	int		i;
	int		size;
	char	**str;
	t_lexer	*temp;

	size = 0;
	temp = parser->command->next;
	if (!parser)
	{
		free_data(prompt);
		exit(g_code);
	}
	while (temp && ++size)
		temp = temp->next;
	str = (char **)ft_calloc(size + 1, sizeof(char *));
	temp = parser->command->next;
	i = 0;
	while (temp)
	{
		str[i] = ft_strdup(temp->content);
		temp = temp->next;
		i++;
	}
	return (ms_exit_success(prompt, str));
}
