/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-ppe <joao-ppe@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 15:20:21 by joao-ppe          #+#    #+#             */
/*   Updated: 2024/02/28 15:20:21 by joao-ppe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	next_char(char *str)
{
	int		i;

	i = -1;
	while (str[++i])
	{
		if ((!ft_isalpha(str[i]) && !ft_isdigit(str[i]))
			|| str[i] == ' ')
			return (str[i]);
	}
	return (0);
}

char	*copy_content(char *new_str, char *input)
{
	char	*new; // nova string para onde se copia tudo

	new = NULL;
	if (!new_str) // new_str eh a string antiga
		new = ft_strndup(input, ft_strclen(input, '$') - 1);
 		new = ft_strjoin(new_str, " ");
	else
		new = ft_strjoin(new_str, ft_strndup(input, ft_strclen(input, next_char(input)) - 1));
	if (new_str)
		free (new_str);
	return (new);
}

static char	*find_value(char *key, t_env_list *env_list)
{
	t_env_list	*head;

	head = env_list;
	
	while (head != NULL)
	{
		if (!ft_strncmp(key, head->key, ft_strlen(head->key)))
			return (ft_strdup(head->value));
		head = head->next;
	}
	return (NULL);
}

char	*get_key_value(char *new_str, char *input, t_env_list *env_list)
{
	char	*key;
	char	*value;
	char	*new;

	new = NULL;
	if (*(input + 1) == '\0') // no caso do input ser por exemplo só "$" ou "echo $", sem qualquer tipo de key ou conteudo depois do cifrão
		return (ft_strjoin(new_str, "$"));
	else if (*input == '$' && ft_isdigit(*(input + 1))) // no caso de haver numeros depois do 
		return (handle_digits(new_str, input));
	key = ft_strndup(input + 1, ft_strclen(input + 1, ' ')); // isolar a key do INPUT, sem espacos e $ para ser mais facil comparar na lista do ENV
	value = find_value(key, env_list);
	if (new_str && value)
		new = ft_strjoin(new_str, value);
	else if (!new_str && value)
		new = ft_strdup(value);
	else if (!new_str && !value)
		new = ("");
	else if (new_str && !value)
		new = ft_strdup(new_str);
	if (key)
		free (key);
	if (new_str)
		free (new_str);
	if (value)
		free (value);
	return (new);
}

char	*handle_digits(char *new_str, char *input)
{
	char	*temp;
	char	*new;

	new = NULL;
	temp = NULL;
	input = input + 2;
	if (input)
		temp = ft_strndup(input, ft_strclen(input, next_char(input)) - 1);
	if (new_str)
	{
		new = ft_strjoin(new_str, temp);
	}
	else
		new = ft_strdup(temp);
	if (new_str)
		free (new_str);
	return (new);
}

bool	sign_exists(char *str, char c)
{
	int	i;
	
	i = -1;
	while (str[++i])
	{
		if (str[i] == c)
			return (true);
	}
	return (false);
}

int	count_signs(char *str, char c)
{
	int	i;

	i = 0;
	while (str[i] == c)
		i++;
	return (i);
}
