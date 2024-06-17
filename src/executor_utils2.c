/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-ppe <joao-ppe@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 14:28:37 by joao-ppe          #+#    #+#             */
/*   Updated: 2024/06/17 18:01:10 by joao-ppe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	**convert_env(t_env_list *env_list)
{
	int			size;
	char		**env_array;
	t_env_list	*head;

	size = env_list_size(env_list);
	env_array = (char **)ft_calloc(size + 1, sizeof(char *));
	if (!env_array)
		return (NULL);
	head = env_list;
	populate_env_array(env_array, head);
	return (env_array);
}

void	populate_env_array(char **env_array, t_env_list *head)
{
	int		i;
	char	*temp;

	i = 0;
	while (head)
	{
		temp = ms_safejoin(ft_strdup(head->key), NULL);
		temp = ms_safejoin(temp, ft_strdup("="));
		if (head->value && *head->value)
			temp = ms_safejoin(temp, ft_strdup(head->value));
		env_array[i] = malloc((ft_strlen(temp) + 1) * sizeof(char));
		if (!env_array[i])
		{
			free_array(env_array);
			return ;
		}
		ft_strlcpy(env_array[i], temp, ft_strlen(temp) + 1);
		ms_free_string(temp);
		head = head->next;
		i++;
	}
}

char	**convert_parser(t_parser *parser)
{
	char	**parser_array;
	size_t	size;
	t_lexer	*head;
	int		i;

	i = 0;
	size = lexer_list_size(parser->command);
	head = parser->command;
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

int	lexer_list_size(t_lexer *lexer)
{
	int		count;
	t_lexer	*current;

	count = 0;
	current = lexer;
	while (current != NULL)
	{
		count++;
		current = current->next;
	}
	return (count);
}

int	env_list_size(t_env_list *env_list)
{
	int			count;
	t_env_list	*current;

	count = 0;
	current = env_list;
	while (current != NULL)
	{
		count++;
		current = current->next;
	}
	return (count);
}
