/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaraden <mkaraden@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 16:04:51 by mkaraden          #+#    #+#             */
/*   Updated: 2023/04/04 23:29:55 by mkaraden         ###   ########.fr       */
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

//---------LEXER-------------------------------------------//


typedef enum 
{
	TOKEN_PIPE,
	TOKEN_I,
	TOKEN_O,
	TOKEN_I_I,
	TOKEN_O_O,
	TOKEN_WORD,
	TOKEN_EOF
}				TokenType;

typedef struct
{
	TokenType	type;
	char		*value;
}				Token;

Token	*get_next_token(const char **input);	//bunu kullan gec

Token	*generate_word_token(const char **input);	//word tokenler
Token	*generate_pr_token(const char **input);		//pipe redirect tokenleri
char	*create_word(const char **input, const char *start);
Token	*create_token(TokenType type, const char **input, const char *start);
void	free_token(Token *token);

///////////////////////////////////////////////////////////////

/*
	TOKEN_DOUBLE_QUOTE,
	TOKEN_SINGLE_QUOTE,
	TOKEN_ENV_VAR,
*/


//---------PARSER-------------------------------------------//
typedef struct Node
{
	char		**args;
	int			arg_count;
	
	
	char		**infile;
	char		**outfile;
	
	int			inf_count;
	int			out_count;

	struct Node	*next;
}				Node;


Node			*get_parsed(const char **input);

void			parse_word(Node *iter, Token *token);
void			parse_input(Node *iter, Token *token, const char **input);
void			parse_output(Node *iter, Token *token, const char **input);

Node			*create_node();
void			print_parser(Node *head);
void			expand_parsed_nodes(Node *head);



///////////////////////////////////////////////////////////////

//EXPANDER


char *get_expanded(const char *input);


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

EnvNode *env_list; //GLOBAL

EnvNode	*load_environment(char *envp[]);	//Basta calisacak
char	**get_env_arr(EnvNode *head);		//guncel nodeleri **arr olarak dondurur.

void	add_env_node(EnvNode **head, const char *key, const char *value);
void	update_env_node(EnvNode *head, const char *key, const char *new_value);
void	delete_env_node(EnvNode **head, const char *key);
char	*get_env_val(const char *key);
EnvNode	*find_env_node(EnvNode *head, const char *key);

EnvNode *create_env_node(const char *key, const char *value);
void	free_list(EnvNode *head);
void	print_list(EnvNode *head);


//utils

void *ft_realloc(void *ptr, size_t old_size, size_t new_size); //duzelt
void printDoubleArr(char **arr);



#endif