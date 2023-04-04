/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaraden <mkaraden@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 16:04:51 by mkaraden          #+#    #+#             */
/*   Updated: 2023/04/04 16:27:01 by mkaraden         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <ctype.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <readline/readline.h>
# include <readline/history.h>

//LEXER

typedef enum
{
	TOKEN_PIPE,
	TOKEN_LESS,
	TOKEN_LESS_LESS,
	TOKEN_GREATER,
	TOKEN_GREATER_GREATER,
	TOKEN_DOUBLE_QUOTE,
	TOKEN_SINGLE_QUOTE,
	TOKEN_ENV_VAR,
	TOKEN_WORD,
	TOKEN_EOF
}				TokenType;

typedef struct
{
	TokenType	type;
	char		*value;
}				Token;

Token			*create_token(TokenType type, const char *value);
void			free_token(Token *token);
char			*read_word(const char **input);
Token			*next_token(const char **input);

//PARSER
typedef enum
{
	NODE_NONE, //**
	NODE_COMMAND,
	NODE_PIPE,
	NODE_REDIRECT_IN,
	NODE_REDIRECT_OUT,
	NODE_REDIRECT_APPEND,
	NODE_REDIRECT_HEREDOC
}				NodeType;

typedef struct Node
{
	NodeType	type;
	
	char		**args;
	int			num_args;
	
	
	char		**infile;
	char		**outfile;
	
	int			inf_count;
	int			out_count;

	struct Node	*next;
}				Node;

Node			*create_node2(NodeType type, const char *args);
Node			*create_node();
void			free_node(Node *node);

void			print_parser(Node *head);


Node			*parse_command(const char **input);
Node			*parse_redirect(const char **input);
Node			*parse_pipe(const char **input);
Node			*parse(const char *input);
void			print_ast(Node *node, int depth);

Node	*parse_main(const char **input);

//EXPANDER

//PROMPT

void	print_prompt();
int		takeInput(char** str);

//ENV

typedef struct env_node
{
	char *key;
	char *value;
	struct env_node *next;
} EnvNode;

EnvNode	*load_environment(char *envp[]);	//Basta calisacak
char	**get_env_arr(EnvNode *head);		//guncel nodeleri **arr olarak dondurur.

void	add_env_node(EnvNode **head, const char *key, const char *value);
void	update_env_node(EnvNode *head, const char *key, const char *new_value);
void	delete_env_node(EnvNode **head, const char *key);

EnvNode *create_env_node(const char *key, const char *value);
void	free_list(EnvNode *head);
void	print_list(EnvNode *head);



#endif