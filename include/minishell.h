/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-ppe <joao-ppe@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 18:06:27 by pmagalha          #+#    #+#             */
/*   Updated: 2024/05/09 18:30:40 by joao-ppe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define SYNTAX 

/*------------- Libraries ---------------*/

# include "libft/libft.h"
# include <stdbool.h>
# include <stddef.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdarg.h>
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <fcntl.h>

extern int	g_code;

typedef enum s_type
{
	OTHER,
	REDIR_IN,
	REDIR_OUT,
	REDIR2_OUT,
	HEREDOC,
	PIPE,
}				t_type;

typedef struct s_lexer
{
	char			*content;
	t_type			type;
	struct s_lexer	*next;
	struct s_lexer	*prev;
}					t_lexer;

typedef struct s_env_list
{
	char				*key;
	char				*value;
	struct s_env_list	*next;
}						t_env_list;

typedef struct s_parser
{
	t_lexer			*command;
	t_lexer			*redirects;
	char			*builtin;
	char			*hd_file;
	struct s_parser	*next;
	struct s_parser	*prev;
}					t_parser;

typedef struct s_prompt
{
	char		**env;
	t_env_list	*env_list;
	t_lexer		*lexer;
	t_lexer		**token_lst;
	t_parser	*parser;
	bool		interactive;
	bool		quotes[2];
	int			*pid;
	int			pid_size;
	bool		reset;
}				t_prompt;

// BUILTINS.C
int			ms_pwd(void);
int			ms_echo(t_parser *parser);
void		print_ms_env(t_env_list *temp);
int			ms_env(t_prompt *prompt, t_parser *parser);
char		*get_env(t_prompt *prompt, char *path);
int			ms_cd(t_prompt *prompt, t_parser *parser);
int			ms_exit_success(t_prompt *prompt, char **str);
int			ms_exit(t_parser *parser, t_prompt *prompt);
int			exec_builtins(t_prompt *prompt, t_parser *parser);
int			ms_cd_error(void);
// ENV.C
int			count_lines(char **env);
t_env_list	*create_key_value(char *key, char *value);
void		token_add_back_env(t_env_list **env_list, t_env_list *new);
void		set_env_from_strings(int shlvl, char **env, t_prompt *prompt);
void		set_default_env(t_prompt *prompt);
void		set_env(char **env, t_prompt *prompt);
void		print_env_list(t_env_list *head);
// ERROR.C
void		ms_error(int error);
char		*file_dir_error(char *tmp);
int			cmd_not_found(t_parser *parser);
// EXECUTOR.C
void		exit_builtin(t_prompt *prompt, t_parser *parser);
int			fork_parser(t_prompt *prompt, t_parser *parser,
				int fd_in, int end[2]);
void		execute(t_prompt *prompt);
int			exec_path(t_prompt *prompt, t_parser *parser, char **paths);
int			handle_command(t_prompt *prompt, t_parser *parser);
void		single_command(t_prompt *prompt, t_parser *parser);
int			execute_dpath(t_prompt *prompt, t_parser *parser);
// EXECUTOR_UTILS1.C
int			dup_parser(t_prompt *prompt, t_parser *parser,
				int fd_in, int end[2]);
void		wait_pipe(t_prompt *prompt, int *pid);
char		**get_paths(t_prompt *prompt);
int			lexer_list_size(t_lexer *lexer);
int			env_list_size(t_env_list *env_list);
// EXECUTOR_UTILS2.C
char		**convert_env(t_env_list *env_list);
char		**convert_parser(t_parser *parser);
// REDIRECTS.C
int			check_fd(t_parser *parser, int end[2]);

// LEXER
char		*create_content(int i, t_env_list *env_list, char *content);
int			check_content(char *content, char *new_content);
char		*get_operator(char *content);
char		*other_content(char *content);
char		*get_token_content(t_prompt *prompt, char *content);
char		*get_quoted_content(t_prompt *prompt, char *input);
int			get_token(char *input, t_prompt *prompt);
t_type		get_type(char *content);
int			check_pipe(char *content);
//LEXER LIST AUX
int			pipe_error(void);
t_lexer		*create_node(char *content, t_type type);
void		token_add_back(t_lexer **token_lst, t_lexer *new);

//INIT
int			init_pid(t_prompt *prompt);
t_lexer		*init_lexer(t_lexer *lexer);
t_parser	*init_parser(t_parser *parser);
t_env_list	*init_env(t_env_list *env_list);
t_prompt	*init(t_prompt *prompt, char **env);

//ENV
int			count_lines(char **env);
char		**duplicate_env(char **env);
void		update_shlvl(t_prompt *prompt);

//ENV LIST AUX
void		print_env(char **env);
void		print_env_list(t_env_list *head);
void		set_env(char **dup_env, t_prompt *prompt);
void		token_add_back_env(t_env_list **env_list, t_env_list *new);
t_env_list	*create_key_value(char *key, char *value);

//PARSER
bool		redirects_error(t_prompt *prompt, char *msg);
char		ms_count_words(t_prompt *prompt);
int			count_pipes(t_lexer *lexer);
int			check_quotes(char *str);
int			check_dquotes(char *str);
int			check_squotes(char *str);
char		*trim_quotes(char *string);
void		get_parser(t_prompt *prompt);
void		add_parser_back(t_parser **token_lst, t_parser *new);
char		*get_builtin(t_prompt *prompt);
char		get_first_quote(char *str);
void		get_command(t_prompt *prompt);
t_parser	*create_pnode(t_lexer *command, char *builtin, t_lexer *redirects);
void		get_redirects(t_prompt *prompt);
bool		has_consecutive_redirects(t_prompt *prompt);

//Frees
void		free_parser_list(t_parser **parser);
void		free_lexer_list(t_lexer **lexer);
void		free_env_list(t_env_list **parser);
void		free_data(t_prompt *prompt);

//Expander
char		*expander(char *content, t_env_list *env_list, char *new);
int			expand_quoted_sign(char *input, char **newinput,
				t_env_list *env_list);

// Expander Quotes
char		*expand_quoted_variable(char *key, t_env_list *env_list);
char		*get_key(char *input);
char		*expand_single_quotes(char *input);

//Expander Utils
bool		sign_exists(char *str, char sign, char c);
char		*copy_content(char *new_str, char *input, char c);
char		*get_key_value(char *new_str, char *input, t_env_list *env_list);
char		*expand_digits(char *new, char *input);
char		*expand_quotes(char *input, t_env_list *env_list);
char		next_char(char *str);
char		next_char_space(char *str);
char		*ms_safejoin(char *str1, char *str2);
int			is_identifier(char c);
char		*find_value(char *key, t_env_list *env_list);
void		expand_signs(char **input, char **new, t_env_list *env_list);
int			expand_single_quoted_content(char **newinput, char **new);
int			check_quoted_content(char **newinput, char **new);
int			copy_quoted_content(char *newinput, char **new);
//Builtins
int			ms_pwd(void);
void		create_pwd(t_prompt *prompt);
int			ms_echo(t_parser *parser);
int			ms_env(t_prompt *prompt, t_parser *parser);
int			exec_builtins(t_prompt *prompt, t_parser *parser);
char		*get_env(t_prompt *prompt, char *path);
int			ms_exit(t_parser *parser, t_prompt *prompt);
void		free_array(char **arr);

// Export
void		export_error(char *content);
void		print_export(t_env_list *head);
void		insert(t_env_list **head, char *key, char *value);
int			ms_export(t_prompt *prompt, t_parser *parser);
void		insert_sorted(t_env_list **head, t_env_list *node);
int			add_value(char *variable, t_prompt *prompt);
void		add_on_env_list(t_env_list *env_list, char *key, char *value);
char		*extract_key(char *variable);
char		*extract_value(char *variable);
void		extract_variables(char *variable, char *key, char *value);
int			check_export(t_prompt *prompt, t_parser *parser);
t_env_list	*create_dup(t_env_list *env_list);
int			update_existing_value(char *key, char *value, t_env_list *current);

// Export Utils
void		sort_export(t_env_list *dup_env);
// Unset
int			remove_value(char *variable, t_prompt *prompt);
int			delete_env(t_env_list *node);
int			ms_unset(t_prompt *prompt, t_parser *parser);
void		swap(t_env_list *node1, t_env_list *node2);

// Free Utils
void		ms_free_string(char *str);
void		print_parser(t_prompt *prompt);
void		reset_data(t_prompt *prompt);

// Executor
char		*get_hd_input(t_prompt *prompt);
void		execute(t_prompt *prompt);
void		single_command(t_prompt *prompt, t_parser *parser);
int			handle_redirects(t_prompt *prompt, t_parser *parser);
int			set_fd_in(t_prompt *prompt, t_lexer *redir);
int			set_fd_out(t_prompt *prompt, t_lexer *redir);
int			set_hdfile_in(char *file);
bool		get_delimiter(t_lexer *redir, char **delimiter);
int			set_heredoc(t_prompt *prompt, t_parser *parser);
void		send_heredoc(t_prompt *prompt, t_lexer *redir, int fd);
void		print_hd(char *input, int fd);
int			create_hdfile(t_prompt *prompt, char *file);
char		*get_hdfile(t_lexer *redir);
int			handle_command(t_prompt *prompt, t_parser *parser);
void		populate_env_array(char **env_array, t_env_list *head);

//SIGNALS.C
void		execute_sig(void *baseprompt, int sig);
void		init_sign(void);
void		set_signals(t_prompt *prompt);
void		handle(int sig);

//ERROR HANDLING
void		ms_error(int error);

//DEBUGGING
int			dev_mod(t_prompt *prompt);

#endif