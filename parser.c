/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmagalha <pmagalha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 12:18:20 by pmagalha          #+#    #+#             */
/*   Updated: 2024/01/30 15:51:11 by pmagalha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	get_parser(t_prompt *prompt)
{
	int		pipe_count;
	char	*builtin;
	char	**command;
	t_lexer	*start;
	
	builtin = NULL;
	command = NULL;
	start = prompt->lexer;
	// meter aqui o expander
	if (!prompt->lexer)
		printf("empty lexer");
	pipe_count = count_pipes(prompt->lexer);
	while (pipe_count-- >= 0)
	{
		
		while (prompt->lexer && prompt->lexer->type != PIPE)
		{
			check_quotes(prompt->lexer->content);
			if (!builtin)
				builtin = get_builtin(prompt);
			command = get_command(prompt);
			//string = trim_quotes(prompt->lexer->content);
			//redirects = get_redirects(prompt->lexer->type);
			if (prompt->lexer)
				prompt->lexer = prompt->lexer->next;
		}
		add_parser_back(&(prompt->parser), create_pnode(command, builtin, NULL));
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

char	**get_command(t_prompt *prompt)
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
}

	//ate pipe redirect ou built in tenho de contar nr de palavras para fazer o array e depois fazer o mesmo mas a guardar as palavras no array
	//if builtin ou redirect sai fora
	// so um pipe da erro, nao esquecer de meter essa condicao