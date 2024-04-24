/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-ppe <joao-ppe@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 14:28:37 by joao-ppe          #+#    #+#             */
/*   Updated: 2024/04/24 14:39:55 by joao-ppe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**convert_env(t_env_list *env_list) // reduzir linhas
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
	(void)prompt;
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
