/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmagalha <pmagalha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 12:18:20 by pmagalha          #+#    #+#             */
/*   Updated: 2024/01/23 19:11:05 by pmagalha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_parser    parsing(t_prompt prompt)
{
    check_quotes(prompt->lexer); // check quotes eh para checkar se as quotes sao impares ou pares
    expander(prompt->env_list, prompt->lexer); // expander eh uma funcao que o joao vai fazer, que altera as env variables. 
    trim_input(prompt->lexer); // vai remover as quotes
    parser(prompt->lexer); // o parser em si vai, depois de todos os nodes anteriores terem sido tratados (sem quotes e o clrh), vai dividir as coisas em categorias, por exemplo
    // o echo vai ser um built-in e uma string "ola" vai ser o content
    // o parser vai criar um node para cada processo, que depende do numero de pipes. Ao fazer o parser, temos de contar o numero de pipes e assim vamos perceber quantos comandos sao, e temos de criar um node para cada
    // temos de ter um caso no parser para se tiver 3 pipes (echo ||| pwd) dar erro: (syntax error near unexpected token `|')

    return (parser);
}

// o joao vai tirar o nome "list" das cenas relacionadas com env
// o expander vai alterar as variaveis do env
// o export eh um built in que vai fazer o mesmo

// casos para ter em atencao:
