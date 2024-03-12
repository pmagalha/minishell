/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 10:39:05 by pmagalha          #+#    #+#             */
/*   Updated: 2024/03/12 15:01:40 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int check_quotes(char *str) 
{
    int num_quotes = 0;
    bool inside_squotes = false;
	bool inside_dquotes = false;

	printf("%s\n", str);
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
	printf("%d\n", num_quotes);
	if (num_quotes % 2 == 0)
        return 1;
	printf("Error in check quotes: Uneven number of quotes\n");
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

// erro em Content: [''"test"'']