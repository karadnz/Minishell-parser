/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaraden <mkaraden@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 16:24:26 by mkaraden          #+#    #+#             */
/*   Updated: 2023/03/25 16:37:40 by mkaraden         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

Node	*create_node(NodeType type, const char *value, Node *left, Node *right)
{
	Node	*node;

	node = (Node *)malloc(sizeof(Node));
	node->type = type;
	node->value = value ? strdup(value) : NULL;
	node->left = left;
	node->right = right;
	return (node);
}

void	free_node(Node *node)
{
	if (node->value)
	{
		free(node->value);
	}
	if (node->left)
	{
		free_node(node->left);
	}
	if (node->right)
	{
		free_node(node->right);
	}
	free(node);
}

Node	*parse_command(const char **input);

Node	*parse_redirect(const char **input)
{
	Token		*token;
	TokenType	type;
	Node		*right;
		NodeType node_type;

	token = next_token(input);
	type = token->type;
	free_token(token);
	if (type == TOKEN_LESS || type == TOKEN_LESS_LESS || type == TOKEN_GREATER
		|| type == TOKEN_GREATER_GREATER)
	{
		right = parse_command(input);
		if (!right)
		{
			fprintf(stderr, "Error: Missing command after redirection\n");
			return (NULL);
		}
		switch (type)
		{
		case TOKEN_LESS:
			node_type = NODE_REDIRECT_IN;
			break ;
		case TOKEN_LESS_LESS:
			node_type = NODE_REDIRECT_OUT;
			break ;
		case TOKEN_GREATER:
			node_type = NODE_REDIRECT_APPEND;
			break ;
		case TOKEN_GREATER_GREATER:
			node_type = NODE_REDIRECT_OUT;
			break ;
		default:
			fprintf(stderr, "Error: Unknown redirection type\n");
			return (NULL);
		}
		return (create_node(node_type, NULL, NULL, right));
	}
	return (NULL);
}

Node *parse_command(const char **input) {
    Token *token = next_token(input);
    Node *head = NULL;
    Node *tail = NULL;

    while (token->type == TOKEN_WORD) {
        Node *command = create_node(NODE_COMMAND, token->value, NULL, NULL);

        if (head == NULL) {
            head = command;
            tail = command;
        } else {
            tail->right = command;
            tail = command;
        }

        free_token(token);
        token = next_token(input);
    }

    *input -= strlen(token->value); // Move input back to the unprocessed part
    free_token(token);

    return head;
}

Node *parse_pipe(const char **input) {
    Node *left = parse_command(input);

    if (!left) {
        return NULL;
    }

    Token *token = next_token(input);

    if (token->type == TOKEN_PIPE) {
        free_token(token);
        Node *right = parse_pipe(input);

        if (!right) {
            fprintf(stderr, "Error: Missing command after pipe\n");
            free_node(left);
            return NULL;
        }

        return create_node(NODE_PIPE, NULL, left, right);
    } else if (token->type == TOKEN_LESS || token->type == TOKEN_LESS_LESS ||
               token->type == TOKEN_GREATER || token->type == TOKEN_GREATER_GREATER) {
        NodeType node_type;

        switch (token->type) {
            case TOKEN_LESS:
                node_type = NODE_REDIRECT_IN;
                break;
            case TOKEN_LESS_LESS:
                node_type = NODE_REDIRECT_OUT;
                break;
            case TOKEN_GREATER:
                node_type = NODE_REDIRECT_APPEND;
                break;
            case TOKEN_GREATER_GREATER:
                node_type = NODE_REDIRECT_OUT;
                break;
            default:
                fprintf(stderr, "Error: Unknown redirection type\n");
                return NULL;
        }

        free_token(token);
        Node *right = parse_command(input);

        if (!right) {
            fprintf(stderr, "Error: Missing command after redirection\n");
            free_node(left);
            return NULL;
        }

        Node *redirect_node = create_node(node_type, NULL, left, right);
        return redirect_node;
    } else {
        free_token(token);
    }

    return left;
}

Node	*parse(const char *input)
{
	return (parse_pipe(&input));
}

void	print_ast(Node *node, int depth)
{
	if (node)
	{
		for (int i = 0; i < depth; i++)
		{
			printf("  ");
		}
		printf("Node type: %d, value: %s\n", node->type, node->value);
		print_ast(node->left, depth + 1);
		print_ast(node->right, depth + 1);
	}
}
