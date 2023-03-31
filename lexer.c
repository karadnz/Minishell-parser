/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaraden <mkaraden@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 16:08:53 by mkaraden          #+#    #+#             */
/*   Updated: 2023/03/31 16:54:12 by mkaraden         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

Token	*create_token(TokenType type, const char *value)
{
	Token	*token;

	token = (Token *)malloc(sizeof(Token));
	token->type = type;
	token->value = NULL;
	if (value != NULL)
		token->value = strdup(value);
	//token->value = value ? strdup(value) : NULL;
	return (token);
}

void	free_token(Token *token)
{
	if (token->value)
		free(token->value);
	free(token);
}

char	*read_word(const char **input)
{
	const char	*start;
	size_t		length;
	char		*word;

	start = *input;
	while (**input && !strchr("<>|", **input) && !isspace(**input)) //!strchr("<>|'\"", **input)
	{
		(*input)++;
	}
	length = *input - start;
	word = (char *)malloc(length + 1);
	strncpy(word, start, length);
	word[length] = '\0';
	return (word);
}

char	*read_quote_word(const char **input, char quote)
{
	const char	*start;
	size_t		length;
	char		*word;

	start = *input;
	while (**input && (**input != quote)) //BITMEDIGI ZAMANA BAK
	{
		if (!**input)
		{
			printf("invalid quote");
			exit(1);
		}
		(*input)++;
	}
	
	length = *input - (start--);
	length += 2;
	if(**input == quote)
		(*input)++; //SON GORDUGU " I GECMESI ICIN BITMEDIGI ZAMANA BAK
	word = (char *)malloc(length + 1);
	strncpy(word, start, length);
	word[length] = '\0';
	return (word);
}

Token	*next_token(const char **input)
{
	char	*word;
	Token	*token;
	
	while (isspace(**input))
	{
		(*input)++;
	}

	if (**input == '\0')
	{
		return (create_token(TOKEN_EOF, NULL));
	}

	switch (**input)
	{
	case '|':
		(*input)++;
		return (create_token(TOKEN_PIPE, "|"));
	case '<':
		(*input)++;
		if (**input == '<')
		{
			(*input)++;
			return (create_token(TOKEN_LESS_LESS, "<<"));
		}
		return (create_token(TOKEN_LESS, "<"));
	case '>':
		(*input)++;
		if (**input == '>')
		{
			(*input)++;
			return (create_token(TOKEN_GREATER_GREATER, ">>"));
		}
		return (create_token(TOKEN_GREATER, ">"));
	/*case '"':
		(*input)++;
		word = read_quote_word(input, '"');
		token = create_token(TOKEN_WORD, word);
		free(word);
		return (token);
		//return (create_token(TOKEN_DOUBLE_QUOTE, "\""));
	case '\'':
		(*input)++;
		word = read_quote_word(input, '\'');
		token = create_token(TOKEN_WORD, word);
		free(word);
		return (token);
		//return (create_token(TOKEN_SINGLE_QUOTE, "'"));*/
	case '$':
		(*input)++;
		return (create_token(TOKEN_ENV_VAR, "$"));
	default:
	{
		word = read_word(input);
		token = create_token(TOKEN_WORD, word);
		free(word);
		return (token);
	}
	}
}