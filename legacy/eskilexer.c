/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaraden <mkaraden@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 16:08:53 by mkaraden          #+#    #+#             */
/*   Updated: 2023/04/04 20:13:44 by mkaraden         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

Token	*get_next_token(const char **input)
{
	char	*word;
	Token	*token;

	while (isspace(**input))
	{
		(*input)++;
	}
	if (**input == '\0')
		return (create_token(TOKEN_EOF, NULL));
	if (**input == '<' || **input == '>' || **input == '|')
		return (generate_pr_token(input));
	if (**input == '\'' || **input == '"')
		return (generate_quote_token(input));

	word = read_word(input);
	token = create_token(TOKEN_WORD, word);
	free(word);
	return (token);
}

Token	*generate_pr_token(const char **input) //pipe redirect
{
	if (**input == '|')
	{
		(*input)++;
		return (create_token(TOKEN_PIPE, "|"));
	}
	else if (**input == '<')
	{
		(*input)++;
		if (**input == '<')
		{
			(*input)++;
			return (create_token(TOKEN_LESS_LESS, "<<"));
		}
		return (create_token(TOKEN_LESS, "<"));
	}
	else if (**input == '>')
	{
		(*input)++;
		if (**input == '>')
		{
			(*input)++;
			return (create_token(TOKEN_GREATER_GREATER, ">>"));
		}
		return (create_token(TOKEN_GREATER, ">>"));
	}
}

Token	*generate_quote_token(const char **input) //pipe redirect
{
	char	*word;
	Token	*token;
	
	if (**input == '"')
	{
		(*input)++;
		word = read_quote_word(input, '"');
		word = read_word(input);
		token = create_token(TOKEN_WORD, word);
		free(word);
		return (token);
	}
	if (**input == '\'')
	{
		(*input)++;
		word = read_quote_word(input, '\'');
		token = create_token(TOKEN_WORD, word);
		free(word);
		return (token);
	}
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

	while (**input && !strchr("<>|", **input) && !isspace(**input)) //quote bitince bosluk gorene kadar devam
	{
		(*input)++;
	}
	
	length = *input - (start--);
	length += 2;
	if (**input == quote)
		(*input)++;
	word = (char *)malloc(length + 1);
	strncpy(word, start, length);
	word[length] = '\0';
	return (word);
}

char	*read_word(const char **input) //cumlenin ortasinda tirnak olsa da bosluk gorunce kesiyor
{
	const char	*start;

	start = *input;
	while (**input && !strchr("<>|", **input) && !isspace(**input)) //!strchr("<>|'\"", **input)
	{
		/*if (**input == '"' || **input == '\'')
			skip_quotes(input, **input);*/
		(*input)++;
	}
	return (create_word(input, start));
}

void	skip_quotes(const char **input, const char quote)
{
	while (**input && **input != quote)
	{
		(*input)++;
	}
}

char	*create_word(const char **input, const char *start)
{
	size_t	len;
	char	*word;

	len = *input - start;
	word = (char *)malloc(len + 1);
	strncpy(word, start, len);
	word[len] = '\0';
	return (word);
}

Token	*create_token(TokenType type, const char *value)
{
	Token	*token;

	token = (Token *)malloc(sizeof(Token));
	token->type = type;
	token->value = NULL;
	if (value != NULL)
		token->value = strdup(value);
	return (token);
}

void	free_token(Token *token)
{
	if (token->value)
		free(token->value);
	free(token);
}
