/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmagalha <pmagalha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 18:10:31 by pmagalha          #+#    #+#             */
/*   Updated: 2024/01/23 18:44:52 by pmagalha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//fazere malloc para a struct do prompt
//dar malloc para o lexer e para lexer ptr
// dar init da str a NULL e do token a NULL

#include "minishell.h"

t_prompt    *init(t_prompt *prompt, char **env)
{    
    prompt = malloc(sizeof(t_prompt));
    if (!prompt)
        return (NULL);
    prompt->lexer = NULL;
    prompt->parser = NULL;
    prompt->token_lst = NULL;
    prompt->env_list = NULL;
    prompt->env = env;
    return (prompt);
}

t_lexer     *init_lexer(t_lexer *lexer)
{
    lexer = malloc(sizeof(t_lexer));
    if (!lexer)
        return (NULL);
    lexer->type = 0;
    lexer->content = NULL;
    lexer->next = NULL;
    return (lexer);
}

t_env_list   *init_env(t_env_list *env_list)
{
    env_list = malloc(sizeof(t_env_list));
    if (!env_list)
        return NULL;
    env_list->full_string = NULL;
    env_list->key = NULL;
    env_list->value = NULL;
    env_list->next = NULL;
    return (env_list);
}