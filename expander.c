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
$? | '$?' | echo '$?' "$?" $? "'$?'" '"$?"' $USER $KILLME - works
echo -n "hello done " "'hello'" 'test' "" "  " $'HOME' >> file |ls<$USER| ls <$USER || $pwd (NA) - works
echo "test'test" 'test"test' "test"test test"test" '$USER"test' "$USER'test" "" '' """" ""test"" ''"test"'' "'test'" '"test"' "''test''" - works but... [""""]
echo -n hello $false > file | cat < $USER > file_2 | pwd || $? | >> test | "" - works but error after last pipe if there's nothing
*/

// VERIFICAR CASO ""boas"" - ta fodido
// VERIFICAR CASO de: "" e ''. uma das opcoes guarda um node nulo, e outra nao guarda nada. mais tarde podemos decidir qual das opcoes eh melhor

// HA LEAKS EM "'test'" e "''test''" (ter double quote e single quote a seguir)

#include "minishell.h"

static char *expand_single_quotes(char *input)
{
    char    *new;

    new = NULL;
	new = copy_content(new, input + 1, *input);
    return (new);
}

// Known bugs: echo "boas $'USER tudo $PWD" - nao esta a imprimir o $ antes do 'USER - fixed
//             echo "boas $'USER $PWD' tudo $HOME" - o $PWD tem que ser expandido :facepalm: !!!  FIXED
// echo -n hello $false $USER file | cat $$ $USER$PWD$2LANG $? file_2 | pwd | "$?" | '$LANG' $"test" |
static char *expand_double_quotes(char *input, t_env_list *env_list)
{
    char    *new;
    char    *newinput;
    char    *tmp;
    char    *key;
	char	*tmp2;

    printf("QUOTES INPUT: [%s]\n", input);
    newinput = ft_strndup(input, ft_strclen(input + 1, '"') + 1);
	new = NULL;
    tmp = newinput;
	tmp2 = NULL;
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
            new = ms_safejoin(new, expander(key, env_list));
            newinput += ft_strlen(key);
            ms_free_string(key);
            continue ;
        }
        else if (*newinput != '\"' && *newinput != '\'' && *newinput != '$')
        {
            if (is_identifier(*newinput))
            {
				tmp2 = ft_substr(newinput, 0, 1);
                new = ms_safejoin(new, tmp2);
                newinput++;
                continue ;
            }
            new = copy_content(new, newinput, next_char(newinput + 1));
            newinput += ft_strclen(newinput, next_char(newinput + 1)) - 1;
            if (*newinput == '"')
                newinput++;
        }
        else if (*newinput == '\'' && sign_exists(newinput + 1, '\'', '\"'))
        {
			tmp2 = expand_single_quotes(newinput);
            new = ms_safejoin(new, tmp2);
            newinput += ft_strclen(newinput + 1, '\'') + 1;
        }
        else if (*newinput == '\'')
		{
			tmp2 = ft_substr(newinput, 0, 1);
            new = ms_safejoin(new, tmp2);
		}
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
            new_input = expand_double_quotes(input, env_list);
        else if (*input == '\'')
            new_input = expand_single_quotes(input);
    }
    return (new_input);
}

// 'dadawdwad'   $USER $$$PWD       "$USER"'"$USER"' boas?
char    *expander(char *input, t_env_list *env_list)
{
    char    *new;
	char	*tmp;

    new = NULL;
	tmp = NULL;
   // printf("RAW INPUT: [%s]\n", input);
	if (!*(input))
		return (NULL);
    while (*input)
    {
        if (*input != '$' && *input != '\'' && *input != '\"')
        {
            if (is_identifier(*input))
            {
				tmp = ft_substr(input, 0, 1);
                new = ms_safejoin(new, tmp);
                input++;
                continue ;
            }
            new = copy_content(new, input, next_char(input + 1));
            input += ft_strclen(input, next_char(input + 1)) - 1;
        }
        else if (*input == '\'' || *input == '\"')
        {
			tmp = expand_quotes(input, env_list);
            new = ms_safejoin(new, tmp);
			//printf("NEW: [%s] | TEMP: [%s]\n", new, tmp);
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
            //printf("entrei\n");
            new = get_key_value(new, input, env_list);
            input += ft_strclen(input + 1, next_char(input + 1));
        }
        input++;
    }
    return (new);
}