/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmagalha <pmagalha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 12:18:20 by pmagalha          #+#    #+#             */
/*   Updated: 2024/04/19 16:50:35 by pmagalha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// meter erro para quando so tem redirect e nao tem ficheiro seguinte

void print_parser(t_prompt *prompt) {
    int i = 1;
    t_parser *parser = prompt->parser;

    while (parser) {
        printf("\nNODE [%d] = BUILTIN: [%s]\n", i, parser->builtin);

        t_lexer *command = parser->command; // Reset command pointer for each parser node
		t_lexer *redirects = parser->redirects; // Reset command pointer for each parser node
		
        while (command) {
            printf("COMMAND [%d] = COMMAND: [%s]\n", i, command->content);
            command = command->next;
		}
		while (redirects) {
            printf("REDIRECTS [%d] = REDIR: [%s]\n", i, redirects->content);
            redirects = redirects->next;
        }

        parser = parser->next;
        i++;
    }
}

void	get_parser(t_prompt *prompt)
{
	int			pipe_count;
	t_lexer		*start;
	t_parser	*p_start;

	start = prompt->lexer;
	if (!prompt->lexer)
		printf("empty lexer\n");
	add_parser_back(&(prompt->parser), create_pnode(NULL, NULL, NULL));
	p_start = prompt->parser;
	pipe_count = count_pipes(prompt->lexer);
	while (pipe_count-- >= 0)
	{
		while (prompt->lexer && prompt->lexer->type != PIPE)
		{
			if (!prompt->parser->builtin)
				prompt->parser->builtin = get_builtin(prompt);
			get_command(prompt);
			get_redirects(prompt);
		}
		if (prompt->lexer && prompt->lexer->next && prompt->lexer->type == PIPE)
		{
			if (prompt->lexer->next->type == PIPE) // this is the condition for the double pipes case, (it will ignore the second)
				prompt->lexer->next = prompt->lexer->next->next;
			add_parser_back(&(prompt->parser), create_pnode(NULL, NULL, NULL));
			prompt->lexer = prompt->lexer->next;
		}
	}
	prompt->parser = p_start;
	prompt->lexer = start;
}

int	count_pipes(t_lexer *lexer)
{
	int	pipe_count;
	t_lexer	*head;

	pipe_count = 0;
	head = lexer;
	while (head != NULL)
	{
		if (head->type == PIPE)
			pipe_count++;
		if (head->type == PIPE && !head->next) //condition in case there is only one pipe
		{
			printf("minishell: syntax error near unexpected token `|'\n");
			exit (1);
		}
		head = head->next;
	}
	return (pipe_count);
}

char	ms_count_words(t_prompt *prompt)
{
	int		count;
	t_lexer	*temp;

	temp = prompt->lexer;
	count = 0;
	while ((temp && temp->type == OTHER) && get_builtin(prompt) == NULL)
	{
		count++;
		temp = temp->next;
	}
	return (count);
}

/* static bool	is_builtin(char *input)
{
	if (!ft_strncmp(input, "echo", 4)
		|| !ft_strncmp(input, "pwd", 3)
		|| !ft_strncmp(input, "env", 3)
		|| !ft_strncmp(input, "cd", 2)
		|| !ft_strncmp(input, "exit", 4)
		|| !ft_strncmp(input, "export", 6)
		|| !ft_strncmp(input, "unset", 5))
		return (true);
	return (false);
}
 */
void	get_command(t_prompt *prompt)
{
	t_lexer	*command_node;

	command_node = NULL;
	if (!prompt->lexer)
		return ;
	if (prompt->lexer->type == OTHER)
	{
		// comentei isto porque estava a avancar um node do echo
		/* if (is_builtin(prompt->lexer->content) && prompt->lexer->next)
			prompt->lexer = prompt->lexer->next; */
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
			command_node->next->next = NULL;
			command_node->next->prev = command_node;
		}
		prompt->lexer = prompt->lexer->next;
	}
}

void	get_redirects(t_prompt *prompt)
{
	t_lexer	*redirect;
	t_lexer	*new_node;

	redirect = NULL;
	new_node = NULL;
	if (!prompt->lexer)
		return ;
	if (prompt->lexer->type == REDIR_OUT || prompt->lexer->type == REDIR2_OUT
		|| prompt->lexer->type == REDIR_IN || prompt->lexer->type == HEREDOC)
	{ // MUDAR DAQUI O HEREDOC PARA OUTRO SITIO PARA DAR HANDLE DELA ESPECIFICAMENTE
		if (!prompt->lexer->next)
		{
			prompt->parser->redirects = NULL;
			printf("Redirect: No such file or directory\n");
			prompt->lexer = prompt->lexer->next;
			exit (1); // METER ERRO DESSENTE AKI
		}
		if (!prompt->parser->redirects)
			prompt->parser->redirects = create_node(ft_strdup(prompt->lexer->next->content), prompt->lexer->type);
		else if (prompt->parser->redirects)
		{
			new_node = create_node(ft_strdup(prompt->lexer->next->content), prompt->lexer->type);
			token_add_back(&prompt->parser->redirects, new_node);
/* 			redirect = prompt->parser->redirects;
			while (redirect->next != NULL)
				redirect = redirect->next;
			redirect->next = malloc(sizeof(t_lexer));
			redirect->next->type = prompt->lexer->type;
			redirect->next->content = ft_strjoin(prompt->lexer->content, NULL);
			redirect->next->next = NULL;
			redirect->next->prev = redirect; */
		}
		else
			printf("error in redirects\n");
		if (prompt->lexer->next->next)
			prompt->lexer = prompt->lexer->next->next;
		else
			prompt->lexer = NULL;
	}
}
