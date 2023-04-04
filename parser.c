/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaraden <mkaraden@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 20:06:57 by mkaraden          #+#    #+#             */
/*   Updated: 2023/04/04 23:28:44 by mkaraden         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

Node	*create_node()
{
	Node	*node;

	node = (Node *)malloc(sizeof(Node));
	node->arg_count = 0;
	node->inf_count = 0;
	node->out_count = 0;
	node->args = NULL;
	node->outfile = NULL;
	node->infile = NULL;
	node->next = NULL;
	return (node);
}

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
			iter = iter->next;
		}
		else if (token->type == TOKEN_WORD)
			parse_word(iter, token);
		else if (token->type == TOKEN_I || TOKEN_I_I)
			parse_input(iter, token, input);
		else if (token->type == TOKEN_O || TOKEN_O_O)
			parse_output(iter, token, input);
		token = get_next_token(input);
		
	}
	iter->next = NULL;
	return (head);
}

void	parse_word(Node *iter, Token *token)
{
	iter->args = (char **)realloc(iter->args, (iter->arg_count + 1)* sizeof(char *));
	iter->args[iter->arg_count] = strdup(token->value);
	iter->arg_count++;
}

void	parse_input(Node *iter, Token *token, const char **input)
{
	token = get_next_token(input);
	if (token->type == TOKEN_WORD)
	{
		iter->infile = (char **)realloc(iter->infile, (iter->inf_count+ 1)* sizeof(char *));
		iter->infile[iter->inf_count] = strdup(token->value); //gordugun okumaya calis
		iter->inf_count++; 
	}
	else
	{
		printf("INVALID SYNTAX \n");
	}
}

void	parse_output(Node *iter, Token *token, const char **input)
{
	token = get_next_token(input);
	if (token->type == TOKEN_WORD)
	{
		iter->outfile = (char **)realloc(iter->outfile, (iter->out_count+ 1)* sizeof(char *));
		iter->outfile[iter->out_count] = strdup(token->value); //gordugun okumaya calis
		iter->out_count++; 
	}
	else
	{
		printf("INVALID SYNTAX \n");
	}
}

void print_parser(Node *head)
{
	Node *iter = head;

	int i = 0;
	while(iter != NULL)
	{
		//printf("p\n\n");
		printf("Node: %d \n", i);
		//printf("Type: %d \n", iter->type);
		printf("args: ");
		//printf("p\n\n");
		for (int i = 0; i < iter->arg_count; i++)
		{
			printf("%s , ", iter->args[i]);
			//printf("p\n\n");
		}
		printf("\n");

		printf("infile: ");
		//printf("p\n\n");
		for (int i = 0; i < iter->inf_count; i++)
		{
			printf("%s , ", iter->infile[i]);
			//printf("p\n\n");
		}
		printf("\n");

		printf("outfile: ");
		//printf("p\n\n");
		for (int i = 0; i < iter->out_count; i++)
		{
			printf("%s , ", iter->outfile[i]);
			//printf("p\n\n");
		}
		printf("\n\n\n");
		//printf("p\n\n");
		iter=iter->next;
		i++;
	}
}

void expand_parsed_nodes(Node *head)
{
	int		i;
	Node	*iter;
	
	iter = head;
	i = 0;
	while(iter != NULL)
	{
		i = 0;
		while (i < iter->arg_count)
		{
			iter->args[i] = get_expanded(iter->args[i]);
			i++;
		}
		i = 0;
		while (i < iter->inf_count)
		{
			iter->infile[i] = get_expanded(iter->infile[i]);
			i++;
		}
		i = 0;
		while (i < iter->out_count)
		{
			iter->outfile[i] = get_expanded(iter->outfile[i]);
			i++;
		}
		iter=iter->next;
	}
}
