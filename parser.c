/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaraden <mkaraden@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 20:06:57 by mkaraden          #+#    #+#             */
/*   Updated: 2023/04/07 16:26:09 by mkaraden         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//input parslanip geri verilir
//yeni bir node sadece pipe varsa olusur
//arrayler pipe gorunce null terminatelenir *** 

Node	*get_parsed(const char **input)
{
	Node	*head;
	Node	*iter;
	Token	*token;

	head = create_node();
	iter = head;
	token = get_next_token(input);
	while (token->type != TOKEN_EOF)
	{
		if (token->type == TOKEN_PIPE)
		{
			iter->next = create_node();
			null_terminate_arrs(iter);
			iter = iter->next;
		}
		else if (token->type == TOKEN_WORD)
			parse_word(iter, token);
		else if (token->type == TOKEN_I || token->type == TOKEN_I_I)
			parse_input(iter, token, input);
		else if (token->type == TOKEN_O || token->type == TOKEN_O_O)
			parse_output(iter, token, input);
		free_token(&token);
		token = get_next_token(input);
	}
	iter->next = NULL;
	free_token(&token);
	return (head);
}

Node	*create_node(void)
{
	Node	*node;

	node = (Node *)malloc(sizeof(Node));
	node->arg_count = 0;
	node->inf_count = 0;
	node->out_count = 0;
	node->args = NULL;
	node->next = NULL;
	node->outfile = NULL;
	node->infile = NULL;
	return (node);
}

void	parse_word(Node *iter, Token *token)
{
	iter->args = (char **)ft_realloc(iter->args,
			(iter->arg_count), sizeof(char *));
	iter->args[iter->arg_count] = strdup(token->value);
	iter->arg_count++;
}

void	parse_input(Node *iter, Token *token, const char **input)
{
	TokenType	type;

	type = token->type;
	free_token(&token);
	token = get_next_token(input);
	if (token->type == TOKEN_WORD)
	{
		iter->infile = (s_file **)ft_realloc(iter->infile,
				(iter->inf_count), sizeof(s_file *));
		iter->infile[iter->inf_count] = (s_file *)malloc(sizeof(s_file));
		iter->infile[iter->inf_count]->name = strdup(token->value);
		iter->infile[iter->inf_count]->type = type;
		iter->inf_count++;
	}
	else
		printf("INVALID SYNTAX \n");
}

void	parse_output(Node *iter, Token *token, const char **input)
{
	TokenType	type;

	type = token->type;
	free_token(&token);
	token = get_next_token(input);
	if (token->type == TOKEN_WORD)
	{
		iter->outfile = (s_file **)ft_realloc(iter->outfile,
				(iter->out_count), sizeof(s_file *));
		iter->outfile[iter->out_count] = (s_file *)malloc(sizeof(s_file));
		iter->outfile[iter->out_count]->name = strdup(token->value);
		iter->outfile[iter->out_count]->type = type;
		iter->out_count++;
	}
	else
		printf("INVALID SYNTAX \n");
}
