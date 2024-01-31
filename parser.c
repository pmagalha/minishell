/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmagalha <pmagalha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 12:18:20 by pmagalha          #+#    #+#             */
/*   Updated: 2024/01/31 16:29:11 by pmagalha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// meter erro para quando so tem redirect e nao tem ficheiro seguinte

void	get_parser(t_prompt *prompt)
{
	int		pipe_count;
	char	**command;
	t_lexer	*start;
	
	command = NULL;
	start = prompt->lexer;
	if (!prompt->lexer)
		printf("empty lexer");
	expander(prompt->lexer, prompt->env_list);
	pipe_count = count_pipes(prompt->lexer);
	while (pipe_count-- >= 0)
	{
		add_parser_back(&(prompt->parser), create_pnode(NULL, NULL, NULL));
		while (prompt->lexer && prompt->lexer->type != PIPE)
		{
			check_quotes(prompt->lexer->content);
			if (!prompt->parser->builtin)
				prompt->parser->builtin = get_builtin(prompt);
			
			get_command(prompt);			
			
			get_redirects(prompt);
			if (prompt->lexer && prompt->parser->redirects)
				prompt->lexer = prompt->lexer->next;
		}
	}
	prompt->lexer = start;
}

// o expander vai alterar as variaveis do env
// o export eh um built in que vai fazer o mesmo
// o redirect vai sempre guardar o redirect e a palavra que esta a frente


// depois temos de fazer tambem condicoes nesta funcao para redirection e heredocs e tratar do dollar sign (expander)
// o parser vai guardar numa lista ou array de simple comands, que vai ser todos os nodes que estejam depois de um pipe, e ate o pipe seguinte
// dentro dos pipes pode haver varias redirections. Eh importante contar as redirectios

int	count_pipes (t_lexer *lexer)
{
	int	pipe_count;

	pipe_count = 0;
	while (lexer != NULL)
	{
		if (lexer->type == PIPE)
		{
			pipe_count++;
		}
		lexer = lexer->next;
	}
	printf("Number of pipes: %d\n\n", pipe_count);
	return (pipe_count);
}

char	count_words(t_prompt *prompt)
{
	int	count;
	t_lexer *temp;
	
	temp = prompt->lexer;
	count = 0;
	
	while ((temp && temp->type == OTHER) && get_builtin(prompt) == NULL)
	{
		count++;
		temp = temp->next;
	}
	printf("\nWord count in lexer: %d\n\n", count);
	return (count);
}

/* char	**get_command(t_prompt *prompt)
{
	char	**commands;
	int		i;
	
	int word_count;

	if (get_builtin(prompt))
		return (NULL);
	word_count = count_words(prompt);
	commands = ft_calloc(sizeof(char*), word_count + 1);
	if (!commands)
		return (NULL);
	i = 0;
	while (i < word_count)
	{
		printf("Content being copied: %s\n", prompt->lexer->content);
		commands[i] = ft_strdup(prompt->lexer->content);
		prompt->lexer = prompt->lexer->next;
		i++;
	}
	return (commands);
} */

void	get_command(t_prompt *prompt)
{
	t_lexer	*command_node;

	command_node = NULL;
	if (!prompt->lexer)
		return ;
	if (prompt->lexer->type == OTHER)
	{
		if (!prompt->parser->command)
			prompt->parser->command = create_node(ft_strdup(prompt->lexer->content), prompt->lexer->type);
		else
		{
			command_node = prompt->parser->command;
 			while (command_node->next != NULL)
				command_node = command_node->next;
			command_node->next = malloc(sizeof(t_lexer));
			command_node->next->type = prompt->lexer->type;
			command_node->next->content = ft_strdup(prompt->lexer->content);
			printf("parei");
			command_node->next->next = NULL;
			command_node->next->prev = command_node;
		}
		prompt->lexer = prompt->lexer->next;
	}
}

void	get_redirects(t_prompt *prompt)
{
	t_lexer	*redirect;
	
	redirect = NULL;
	if (!prompt->lexer)
		return ;
	if (prompt->lexer->type == REDIR_OUT || prompt->lexer->type == REDIR2_OUT
				|| prompt->lexer->type == REDIR_IN || prompt->lexer->type == HEREDOC)
		{
			if (!prompt->parser->redirects)
				prompt->parser->redirects = create_node(ft_strdup(prompt->lexer->next->content), prompt->lexer->type);
			else
			{
				redirect = prompt->parser->redirects;
				while (redirect->next != NULL)
					redirect = redirect->next;
				redirect->next = malloc(sizeof(t_lexer));
				redirect->next->type = prompt->lexer->type;
				redirect->next->content = strdup(prompt->lexer->next->content);
				redirect->next->next = NULL;
				redirect->next->prev = redirect->next;
			}
			prompt->lexer = prompt->lexer->next;
		}
}

/* char	**get_redirects(t_prompt *prompt, char **redirects)
{
	char	**command;
	char	*redirect;
	char	*file;

	if (!prompt->lexer)
		return (redirects);
	if (prompt->lexer->type == REDIR_OUT || prompt->lexer->type == REDIR2_OUT
				|| prompt->lexer->type == REDIR_IN || prompt->lexer->type == HEREDOC)
		{
			redirect = ft_strdup(prompt->lexer->content);
			prompt->lexer = prompt->lexer->next;
			file = ft_strdup(prompt->lexer->content);
			command = malloc(3 * sizeof(char*)); 
			if (!command)
			{
				free(redirect);
				free(file);
				return (redirects);
			}
			command[0] = redirect;
			command[1] = file;
			command[2] = NULL;
			printf("Redirect is: %s and file is: %s\n", command[0], command[1]);
			return (command);
			}
		return (redirects);
} */

	//ate pipe redirect ou built in tenho de contar nr de palavras para fazer o array e depois fazer o mesmo mas a guardar as palavras no array
	//if builtin ou redirect sai fora
	// so um pipe da erro, nao esquecer de meter essa condicao