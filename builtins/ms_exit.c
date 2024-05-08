/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmagalha <pmagalha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 16:03:55 by pmagalha          #+#    #+#             */
/*   Updated: 2024/05/08 13:00:45 by pmagalha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

static void	exit_code(char **str)
{
	int		exit_code;

	exit_code = 0;
	if (!str[0])
		exit_code = 0;
	else if (str[1])
	{
		ft_putstr_fd("-minishell: exit: too many arguments\n", STDERR_FILENO);
		exit_code = 1;
		free_array(str);
		exit(exit_code);
	}
	else if (str_is_digit(str[0]) || ft_atoi(str[0]))
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
	exit(exit_code);
}

void	ms_exit_success(t_prompt *prompt, char **str)
{
	rl_clear_history();
	ft_putstr_fd("exit\n", STDOUT_FILENO);
	free_data(prompt);
	exit_code(str);
	free_array(str);
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
	ms_exit_success(prompt, str);
	return (EXIT_SUCCESS);
}
