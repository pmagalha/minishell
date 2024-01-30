/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmagalha <pmagalha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 10:39:05 by pmagalha          #+#    #+#             */
/*   Updated: 2024/01/30 12:24:01 by pmagalha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_quotes(char *str)
{
	if (check_dquotes(str) || check_squotes(str))
	{
		printf("Error in check quotes: Uneven nr of quotes");
		return (1);
	}
	else
		return (0);
}

int	check_squotes(char *str)
{
	int	num_squotes;

	num_squotes = 0;
	while (*str)
	{
		if (*str == '\'')
			num_squotes++;
		str++;
	}
	if (num_squotes % 2 == 0)
		return (0);
	else
		return (1);
}

int	check_dquotes(char *str)
{
	int	num_dquotes;

	num_dquotes = 0;
	while (*str)
	{
		if (*str == '"')
			num_dquotes++;
		str++;
	}
	if (num_dquotes % 2 == 0)
		return (0);
	else
		return (1);
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

// erro em Content: [''"test"'']