/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaraden <mkaraden@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 16:04:51 by mkaraden          #+#    #+#             */
/*   Updated: 2023/04/05 05:24:54 by mkaraden         ###   ########.fr       */
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

Token	*get_next_token(const char **input);	//this

Token	*generate_word_token(const char **input);	//word tokenler
Token	*generate_pr_token(const char **input);		//pipe redirect tokenleri
char	*create_word(const char **input, const char *start);
Token	*create_token(TokenType type, const char **input, const char *start);
void	free_token(Token *token);

///////////////////////////////////////////////////////////////

//---------PARSER-------------------------------------------//

typedef struct t_file //DENEYSEL
{
	TokenType		type;
	char			*name;
}				s_file;

typedef struct Node //append heredoc belirteci eklenecek
{
	char		**args;
	int			arg_count;
	
	
	char		**infile;
	char		**outfile;
	
	s_file		**s_infile;
	s_file		**s_outfile;
	
	int			inf_count;
	int			out_count;

	struct Node	*next; 
}				Node;

Node			*get_parsed(const char **input); //inputu parslayip listenin headini verir

void			parse_word(Node *iter, Token *token);
void			parse_input(Node *iter, Token *token, const char **input);
void			parse_output(Node *iter, Token *token, const char **input);

Node			*create_node();
void			print_parser(Node *head);
///////////////////////////////////////////////////////////////

//---------EXPANDER-------------------------------------------//

typedef struct exp_stsh //normu yedim
{
	int		src_i;
	int		rt_i;

	int		src_len;
	int		rt_len;

	char	*rt;
	
} exp_stsh;

void		expand_parsed_nodes(Node *head); //headi ver expandlasin

char		*get_expanded(const char *input);
void		ft_dollarize(const char *str, exp_stsh *stsh);
void		expand_stsh(const char *str, exp_stsh *stsh, bool sf, bool df);
exp_stsh	*get_stsh(const char *str);

///////////////////////////////////////////////////////////////

//---------ENV-------------------------------------------//

typedef struct env_node
{
	char *key;
	char *value;
	struct env_node *next;
} EnvNode;

EnvNode *env_list; //GLOBAL

EnvNode	*load_environment(char *envp[]);	//Basta calisacak
char	**get_env_arr(EnvNode *head);		//guncel nodeleri **arr olarak dondurur.
char	*get_env_val(const char *key);		//key ver value gelsin woaw

void	add_env_node(EnvNode **head, const char *key, const char *value); //ekle
void	update_env_node(EnvNode *head, const char *key, const char *new_value); //update
void	delete_env_node(EnvNode **head, const char *key); //sil

EnvNode	*find_env_node(EnvNode *head, const char *key);

EnvNode *create_env_node(const char *key, const char *value);
void	free_env_list(EnvNode *head);
void	print_list(EnvNode *head);
char	*join_env(const char *key, const char *value);

///////////////////////////////////////////////////////////////




//---------GENEL-------------------------------------------//


//utils

void *ft_realloc(void *ptr, size_t old_size, size_t new_size); //zortlak
void printDoubleArr(char **arr);
void	null_terminate_arrs(Node *iter);
void	free_env_list(EnvNode *head);
void	free_nodes(Node *head);


//PROMPT

void		print_prompt();
int			take_input(char** str);

//DEBUG

void	print_parser_with_token(Node *head);
void	print_parser(Node *head);


#endif