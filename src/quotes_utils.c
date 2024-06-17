/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-ppe <joao-ppe@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 10:39:05 by pmagalha          #+#    #+#             */
/*   Updated: 2024/06/17 18:01:58 by joao-ppe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	check_quotes(char *str)
{
	int		num_quotes;
	bool	inside_squotes;
	bool	inside_dquotes;

	num_quotes = 0;
	inside_squotes = false;
	inside_dquotes = false;
	while (*str)
	{
		if (*str == '\'' && inside_dquotes == false)
		{
			num_quotes++;
			inside_squotes = !inside_squotes;
		}
		if (*str == '"' && inside_squotes == false)
		{
			num_quotes++;
			inside_dquotes = !inside_dquotes;
		}
		str++;
	}
	if (num_quotes % 2 == 0)
		return (1);
	ft_putstr_fd("Error: Uneven number of quotes\n", STDERR_FILENO);
	return (0);
}

char	get_first_quote(char *str)
{
	while (*str)
	{
		if (*str == '\'' || *str == '\"')
			return (*str);
		str++;
	}
	return (*str);
}

char	*trim_quotes(char *string)
{
	char	*res;
	char	quote;
	int		i;
	int		j;
	int		count;

	quote = get_first_quote(string);
	i = -1;
	count = 0;
	while (string[++i])
		if (string[i] == quote)
			count++;
	res = ft_calloc(sizeof(char), i - count + 1);
	i = 0;
	j = 0;
	while (string[i])
	{
		if (string[i] == quote)
			i++;
		else
			res[j++] = string[i++];
	}
	res[j] = '\0';
	return (res);
}
