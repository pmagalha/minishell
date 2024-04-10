/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-ppe <joao-ppe@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 18:06:27 by pmagalha          #+#    #+#             */
/*   Updated: 2024/04/10 16:47:20 by joao-ppe         ###   ########.fr       */
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
#include <signal.h>
#include <sys/wait.h>
# include <sys/stat.h>
# include <fcntl.h>

extern int g_code;

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
    t_type          type;
    struct s_lexer	*next;
    struct s_lexer	*prev;
}	                t_lexer;

typedef struct  s_env_list
{
    char    *key;
    char    *value;
    struct  s_env_list   *next;
}               t_env_list;

typedef struct s_parser
{
    t_lexer *command;
    t_lexer *redirects; // isto eh para guardar as redirections no parser
    char    *builtin;
    char    *hd_file; // heredoc file, que aparentemente sera necessario
    struct s_parser	*next;
    struct s_parser	*prev;
}                   t_parser;

typedef struct  s_prompt
{
    char        **env;
    t_env_list  *env_list;
    t_lexer     *lexer;
    t_lexer		**token_lst;
    t_parser    *parser;
	bool		quotes[2];
    //t_parser    *comands; maybe use this for storing commands between pipes
}               t_prompt;

/*------------- Functions ---------------*/

// Lexer

char	*get_operator(char *content);
char	*other_content(char *content);
char	*get_token_content(t_prompt *prompt, char *content);
char	*get_quoted_content(t_prompt *prompt, char *input);
void    get_token(char *input, t_prompt *prompt);
t_type  get_type(char *content);

//Lexer list aux

t_lexer	*create_node(char *content, t_type type);
void	token_add_back(t_lexer **token_lst, t_lexer *new);

// Init
t_prompt    *init(t_prompt *prompt, char **env);
t_lexer     *init_lexer(t_lexer *lexer);
t_env_list   *init_env(t_env_list *env_list);
t_parser    *init_parser(t_parser *parser);


// Env
int		count_lines(char **env);
char	**duplicate_env(char **env);

// Env list aux
void        print_env(char **env);
void        print_env_list(t_env_list *head);
void        set_env(char **dup_env, t_prompt *prompt);
void    token_add_back_env(t_env_list **env_list, t_env_list *new);
t_env_list	*create_key_value(char *key, char *value);

// Parser
char	ms_count_words(t_prompt *prompt);
int	count_pipes (t_lexer *lexer);
int check_quotes(char *str);
int check_dquotes(char *str);
int check_squotes(char *str);
char	*trim_quotes(char *string);
void   get_parser(t_prompt *prompt);
t_parser *dup_parser(t_lexer *lexer);
void    add_parser_back(t_parser **token_lst, t_parser *new);
char	*get_builtin(t_prompt *prompt);
char	get_first_quote(char *str);

void	get_command(t_prompt *prompt);

t_parser	*create_pnode(t_lexer *command, char *builtin, t_lexer *redirects);
void	get_redirects(t_prompt *prompt);


//Frees
void    free_parser_list(t_parser **parser);
void	free_lexer_list(t_lexer **lexer);
void	free_env_list(t_env_list **parser);
void	free_data(t_prompt *prompt);

//Expander
char	*expander(char *content, t_env_list *env_list);

// Expander Quotes
char	*expand_quoted_variable(char *key, t_env_list *env_list);
char	*get_key(char *input);

//Expander Utils
bool	sign_exists(char *str, char sign, char c);
char	*copy_content(char *new_str, char *input, char c);
char	*get_key_value(char *new_str, char *input, t_env_list *env_list);
char	*expand_digits(char *new, char *input);
char	*expand_quotes(char *input, t_env_list *env_list);
//char	*expand_quoted_content(char *input, char quotes, t_env_list *env_list); // funcao para modificar o new no caso das double quotes?
char	next_char(char *str);
char	next_char_space(char *str);
char	*ms_safejoin(char *str1, char *str2);
int		is_identifier(char c);
char *find_value(char *key, t_env_list *env_list);

//Builtins
int     ms_pwd(void);
int	    ms_echo(t_parser *parser);
int     ms_env(t_prompt *prompt, t_parser *parser);
int 	exec_builtins(t_prompt *prompt, t_parser *parser);
char	*get_env(t_prompt *prompt, char *path);
int     ms_exit(t_parser *parser, t_prompt *prompt);
void	free_array(char **arr);

// Export
void	print_export(t_env_list *head);
void	insert(t_env_list **head, char *key, char *value) ;
int	    ms_export(t_prompt *prompt);
void	insert_sorted(t_env_list **head, t_env_list *node);
void	add_value(char *variable, t_prompt *prompt);
void	add_on_env_list(t_env_list *env_list, char *key, char *value);
char	*extract_key(char *variable);
char	*extract_value(char *variable);
void 	extract_variables(char *variable, char *key, char *value);
int     check_export(t_prompt *prompt);

// Export Utils
void	sort_export(t_env_list *dup_env);
// Unset
int	    remove_value(char *variable, t_prompt *prompt);
int     delete_env(t_env_list *node);
int	    ms_unset(t_prompt *prompt);
void	swap(t_env_list *node1, t_env_list *node2);

// Free Utils
void	ms_free_string(char *str);
void	print_parser(t_prompt *prompt);
void	reset_data(t_prompt *prompt);

// Executor
void    single_command(t_prompt *prompt, t_parser *parser);
int		handle_redirects(t_prompt *prompt);
int		set_fd_in(t_lexer *redir);
int		set_fd_out(t_lexer *redir);

int	dev_mod(t_prompt *prompt);

# endif
