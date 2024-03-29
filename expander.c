/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-ppe <joao-ppe@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 16:07:55 by joao-ppe          #+#    #+#             */
/*   Updated: 2024/02/28 18:31:35 by joao-ppe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
$? | '$?' | echo '$?' "$?" $? "'$?'" '"$?"' $USER $KILLME
echo -n "hello done " "'hello'" 'test' "" "  " $'HOME' >> file |ls<$USER| ls <$USER || $pwd (NA)
echo "test'test" 'test"test' "test"test test"test" '$USER"test' "$USER'test" "" '' """" ""test"" ''"test"'' "'test'" '"test"' "''test''"
echo -n hello $false > file | cat < $USER > file_2 | pwd || $? | >> test | ""
*/

#include "minishell.h"

static char *expand_single_quotes(char *input)
{
    char    *new;

    new = NULL;
    new = ms_safejoin(new, ft_strndup(input + 1, ft_strclen(input + 1, '\'')));
    return (new);
}

// Known bugs: echo "boas $'USER tudo $PWD" - nao esta a imprimir o $ antes do 'USER - TO DO
//             echo "boas $'USER $PWD' tudo $HOME" - o $PWD tem que ser expandido :facepalm: !!!  MAYBE FIXED (?)
// echo -n hello $false $USER file | cat $$ $USER$PWD$2LANG $? file_2 | pwd | "$?" | '$LANG' $"test" |
static char *expand_double_quotes(char *input, t_env_list *env_list)
{
    char    *new = NULL;
    char    *newinput;
    char    *tmp;
    char    *key;

    ////////printf("QUOTES INPUT: [%s]\n", input);
    newinput = ft_strndup(input, ft_strclen(input + 1, '"') + 1);
    tmp = newinput;
    key = NULL;
    while (*newinput)
    {
        if (*newinput == '$') // fazer funcao para passar o &new e modificar diretamente na nova funcao, cuja funcao retorna o tamanho da key ?
        {
            if (*(newinput + 1) == '\'')
            {
                new = ms_safejoin(new, ft_strdup("$'"));
                newinput += 2;
                continue ;
            }
            key = get_key(newinput);
            printf("KEY 2: [%s]\n", key);
            new = ms_safejoin(new, expander(key, env_list));
            newinput += ft_strlen(key);
            ms_free_array(key);
            continue ;
        }
        else if (*newinput != '\"' && *newinput != '\'' && *newinput != '$')
        {
            if (is_identifier(*newinput))
            {
                new = ms_safejoin(new, ft_substr(newinput, 0, 1));
                newinput++;
                continue ;
            }
            new = copy_content(new, newinput, next_char(newinput + 1));
            newinput += ft_strclen(newinput, next_char(newinput + 1)) - 1;
            if (*newinput == '"')
                newinput++;
        }
        else if (*newinput == '\'' && *(newinput - 1) != '$' && sign_exists(newinput + 1, '\'', '\"'))
        {
            new = ms_safejoin(new, expand_single_quotes(newinput));
            newinput += ft_strclen(newinput + 1, '\'') + 1;
        }
        else if (*newinput == '\'')
            new = ms_safejoin(new, ft_substr(newinput, 0, 1));
        if (*newinput != '$')
            newinput++;
    }
    if (tmp)
        free(tmp);
    return (new);
}

char    *expand_quotes(char *input, t_env_list *env_list)
{
    char    *new_input;

    new_input = NULL;
    if (*input == '\"' || *input == '\'')
    {
        if (*input == '\"')
            new_input = expand_double_quotes(input + 1, env_list);
        else if (*input == '\'')
            new_input = expand_single_quotes(input);
    }
    return (new_input);
}

// 'dadawdwad'   $USER $$$PWD       "$USER"'"$USER"' boas?
char    *expander(char *input, t_env_list *env_list)
{
    char    *new;

    new = NULL;
    printf("RAW INPUT: [%s]\n", input);
    while (*input != '\0')
    {
        if (*input != '$' && *input != '\'' && *input != '\"')
        {
            if (is_identifier(*input))
            {
                new = ms_safejoin(new, ft_substr(input, 0, 1));
                input++;
                continue ;
            }
            new = copy_content(new, input, next_char(input + 1));
            input += ft_strclen(input, next_char(input + 1)) - 1;
        }
        else if (*input == '\'' || *input == '\"')
        {
            new = ms_safejoin(new, expand_quotes(input, env_list));
            input += ft_strclen(input + 1, *input) + 1;
        }
        else if (*input == '$' && *(input + 1) == '?')
        {
            new = ms_safejoin(new, ft_itoa(g_code));
            input += 1;
        }
        else if (*input == '$' && *(input + 1) == '$')
        {
            new = ms_safejoin(new, ft_itoa(getpid()));
            input += 1;
        }
        else if (*input == '$')
        {
            printf("entrei\n");
            new = get_key_value(new, input, env_list);
            input += ft_strclen(input + 1, next_char(input + 1));
        }
        input++;
    }
    return (new);
}