/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaraden <mkaraden@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 16:08:53 by mkaraden          #+#    #+#             */
/*   Updated: 2023/03/28 06:19:03 by mkaraden         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

Token	*create_token(TokenType type, const char *value)
{
	Token	*token;

	token = (Token *)malloc(sizeof(Token));
	token->type = type;
	token->value = value ? strdup(value) : NULL;
	return (token);
}

void	free_token(Token *token)
{
	if (token->value)
	{
		free(token->value);
	}
	free(token);
}

char	*read_word(const char **input)
{
	const char	*start;
	size_t		length;
	char		*word;

	start = *input;
	while (**input && !strchr("<>|'\"", **input) && !isspace(**input))
	{
		(*input)++;
	}
	length = *input - start;
	word = (char *)malloc(length + 1);
	strncpy(word, start, length);
	word[length] = '\0';
	return (word);
}

Token	*next_token(const char **input)
{
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
	case '"':
		(*input)++;
		return (create_token(TOKEN_DOUBLE_QUOTE, "\""));
	case '\'':
		(*input)++;
		return (create_token(TOKEN_SINGLE_QUOTE, "'"));
	case '$':
		(*input)++;
		return (create_token(TOKEN_ENV_VAR, "$"));
	default:
	{
		char *word = read_word(input);
		Token *token = create_token(TOKEN_WORD, word);
		free(word);
		return (token);
	}
	}
}