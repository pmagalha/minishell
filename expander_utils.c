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

int is_identifier(char c)
{
    return (c == '|' || c == '<' || c == '>' || c == '[' || c == ']'
        || c == '\'' || c == '\"' || c == ' ' || c == ',' || c == '.'
        || c == ':' || c == '/' || c == '{' || c == '}' || c == '+'
        || c == '^' || c == '%' || c == '#' || c == '@' || c == '!'
        || c == '~' || c == '=' || c == '-' || c == '?' || c == '&' || c == '*');
}

char *ms_safejoin(char *str1, char *str2)
{
    char *new;

    new = NULL;
    if (!str1 && !str2)
        return NULL;
    else if (!str1 && str2)
        new = ft_strdup(str2);
    else if (str1 && !str2)
		new = ft_strdup(str1);
    else
		new = ft_strjoin(str1, str2);
 	ms_free_string(str1);
	ms_free_string(str2);
	//printf("STR 1: [%s] | STR 2: [%s]\n", str1, str2);
    return (new);
}

bool    sign_exists(char *str, char sign, char c)
{
    int i;

    i = 0;
    if (!str || !str[1])
        return (false);
    while (str[i] != '\0' && str[i] != c)
    {
        if (str[i] == sign)
            return (true);
        i++;
    }
    return (false);
}

int count_signs(char *str, char c)
{
    int i;

    i = 0;
    while (str[i] == c)
        i++;
    return (i);
}

char    next_char(char *str) // procura o proximo caracter nao alfanumerico e ignora espacos
{
    int     i;

    i = -1;
    while (str[++i])
    {
        if (!ft_isalnum(str[i]) && str[i] != 32)
            return (str[i]);
    }
    return (0);
}

char    next_char_space(char *str) // procura o proximo caracter nao alfanumerico INCLUINDO espacos
{
    int     i;

    i = -1;
    while (str[++i])
    {
        if (!ft_isalnum(str[i]) || str[i] == 32)
            return (str[i]);
    }
    return (0); 
}

char    *copy_content(char *new_str, char *input, char c)
{
    char    *new;
	char	*temp;

    new = NULL;
	temp = NULL;
	if (input)
		temp = ft_strndup(input, ft_strclen(input, c));
    new = ms_safejoin(new_str, temp);
    return (new);
}

static char *find_value(char *key, t_env_list *env_list)
{
    t_env_list  *head;

    head = env_list;

    while (head != NULL)
    {
        if (!ft_strncmp(key, head->key, ft_strlen(head->key) + 1))
            return (ft_strdup(head->value));
        head = head->next;
    }
    return (NULL);
}

char    *get_key_value(char *new_str, char *input, t_env_list *env_list)
{
    char    *key;
    char    *value;
    char    *new;
	char	*temp;

    new = NULL;
	temp = NULL;
    if (*(input + 1) == '\0') // no caso do input ser por exemplo só "$" ou "echo $", sem qualquer tipo de key ou conteudo depois do cifrão
	{
		temp = ft_strdup("$");
        return (ms_safejoin(new_str, temp));
	}
    else if (*input == '$' && ft_isdigit(*(input + 1))) // no caso de haver numeros depois do 
        return (expand_digits(new_str, input));
    key = ft_strndup(input + 1, ft_strclen(input + 1, next_char_space(input))); // isolar a key do INPUT, sem espacos e $ para ser mais facil comparar na lista do ENV
    input += ft_strclen(input, '$') - 1;
    value = find_value(key, env_list);
	//printf("KEY: [%s] [%ld] | VALUE [%s] [%ld]\n", key, ft_strlen(key), value, ft_strlen(value));
    new = ms_safejoin(new_str, value);
    if (key)
        free (key);
    return (new);
}

char    *expand_digits(char *new_str, char *input)
{
    char    *temp;
    char    *new;

    new = NULL;
    temp = NULL;
    input = input + 2;
    if (input)
        temp = ft_strndup(input, ft_strclen(input, next_char(input)));
    new = ms_safejoin(new_str, temp);
    return (new);
}