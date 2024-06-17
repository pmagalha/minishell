/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_utils1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-ppe <joao-ppe@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 14:12:53 by joao-ppe          #+#    #+#             */
/*   Updated: 2024/06/17 18:01:47 by joao-ppe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	ms_free_string(char *str)
{
	if (str)
		free(str);
}

int	check_content(char *content, char *new_content)
{
	if (new_content == NULL)
	{
		ms_free_string(content);
		return (1);
	}
	ms_free_string(content);
	return (0);
}

char	*ms_safejoin(char *str1, char *str2)
{
	char	*new;

	new = NULL;
	if (!str1 && !str2)
		return (NULL);
	else if (!str1 && str2)
		new = ft_strdup(str2);
	else if (str1 && !str2)
		new = ft_strdup(str1);
	else
		new = ft_strjoin(str1, str2);
	ms_free_string(str1);
	ms_free_string(str2);
	return (new);
}

char	ms_count_words(t_prompt *prompt)
{
	int		count;
	t_lexer	*temp;

	temp = prompt->lexer;
	count = 0;
	while ((temp && temp->type == OTHER) && get_builtin(prompt) == NULL)
	{
		count++;
		temp = temp->next;
	}
	return (count);
}

void	reset_data(t_prompt *prompt)
{
	if (!prompt)
		return ;
	if (prompt->lexer)
		free_lexer_list(&prompt->lexer);
	if (prompt->parser)
		free_parser_list(&prompt->parser);
	if (prompt->pid)
		free(prompt->pid);
	prompt->pid = NULL;
	prompt->lexer = NULL;
	prompt->parser = NULL;
	prompt->reset = true;
}
