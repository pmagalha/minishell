/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmagalha <pmagalha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 18:10:31 by pmagalha          #+#    #+#             */
/*   Updated: 2024/01/12 21:10:57 by pmagalha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//fazere malloc para a struct do prompt
//dar malloc para o lexer e para lexer ptr
// dar init da str a NULL e do token a NULL

#include "minishell.h"

t_prompt    *init(t_prompt *prompt)
{    
    prompt = malloc(sizeof(t_prompt));
    if (prompt == NULL)
        return (NULL);
    prompt->lexer = NULL;
    prompt->parser = NULL;
    prompt->token_lst = NULL;
    return (prompt);
}

t_lexer     *init_lexer(t_lexer *lexer)
{
    lexer->type = 2;
    return (lexer);
}