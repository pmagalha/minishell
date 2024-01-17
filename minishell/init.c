/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmagalha <pmagalha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 18:10:31 by pmagalha          #+#    #+#             */
/*   Updated: 2024/01/17 18:10:07 by pmagalha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//fazere malloc para a struct do prompt
//dar malloc para o lexer e para lexer ptr
// dar init da str a NULL e do token a NULL

#include "minishell.h"

t_prompt    *init(t_prompt *prompt, char **env)
{    
    prompt = malloc(sizeof(t_prompt));
    if (prompt == NULL)
        return (NULL);
    prompt->lexer = NULL;
    prompt->parser = NULL;
    prompt->token_lst = NULL;
    prompt->env_node = NULL;
    prompt->env = env;
    prompt->dup_env = duplicate_env(env);
    return (prompt);
}

t_lexer     *init_lexer(t_lexer *lexer)
{
    lexer = malloc(sizeof(t_lexer));
    lexer->type = 0;
    lexer->content = NULL;
    return (lexer);
}

t_env_node   *init_env(t_env_node *env_node)
{
    env_node = malloc(sizeof(t_env_node));
    env_node->full_string = NULL;
    env_node->key = NULL;
    env_node->value = NULL;
    return (env_node);
}