/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmagalha <pmagalha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 18:06:27 by pmagalha          #+#    #+#             */
/*   Updated: 2024/01/17 18:43:33 by pmagalha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 50
# endif

/*------------- Libraries ---------------*/

#include "libft/libft.h"
#include <stdbool.h>
# include <stddef.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdarg.h>
# include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

/*------------- Structures ---------------*/

typedef enum    s_type
{
	OTHER,
	REDIR_IN,
	REDIR_OUT,
	REDIR2_OUT,
	HEREDOC,
	PIPE,
}	            t_type;

typedef struct s_lexer
{
    char    	    *content;
    int              type;
    struct s_lexer	*next;
    struct s_lexer	*prev;
}	                t_lexer;

typedef struct  s_env_node
{
    char    *key;
    char    *value;
    char    *full_string;
    struct  s_env_node   *next;
}               t_env_node;

// MUDAR ESTA STRUCT DE ENV_NODE PARA ENV_LST PARA EVITAR CAOS

typedef struct s_parser
{
    struct s_parser	*next;
    struct s_parser	*prev;
}                   t_parser;

typedef struct  s_prompt
{
    char        **env;
    char        **dup_env;
    t_env_node  **env_node;
    t_lexer     *lexer;
    t_parser    *parser;
    t_lexer		**token_lst;
}               t_prompt;

/*------------- Functions ---------------*/

void get_token(char *input, t_prompt *prompt);

char    *get_quoted_content(char *content);
char	*get_token_content(char *content);
//void    get_token(char *input, t_prompt lexer);
char	*other_content(char *content);
char	*get_operator(char *content);

// Init
t_env_node   *init_env(t_env_node *env_node);
t_prompt    *init(t_prompt *prompt, char **env);
t_type	get_type(char *content);

//List aux

void free_token_list(t_lexer *head);

t_lexer	*create_node(void *content, t_type type);

void	token_add_back(t_lexer **token_lst, t_lexer *new);
/* void    token_delete_all(t_lexer *token_lst);
t_lexer	*new_token(char *content, t_lexer type);
void	token_add_back(char *content, t_prompt *prompt); */


// Env
int		count_lines(char **env);
int	lines_len(char **env, int line_count);
char	**duplicate_env(char **env);

// para cagar maybe
t_env_node	*create_key_value(char *key, char *value, char *string);
//void print_env_list(t_env_node *head);
//void print_env(char **env);
void get_env(char **dup_env, t_env_node *env_node);

# endif
