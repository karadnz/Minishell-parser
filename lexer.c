/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaraden <mkaraden@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 16:08:53 by mkaraden          #+#    #+#             */
/*   Updated: 2023/04/04 21:28:03 by mkaraden         ###   ########.fr       */
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
		return (create_token(TOKEN_EOF, input, *input)); //NULL
	if (**input == '<' || **input == '>' || **input == '|')
		return (generate_pr_token(input));
	return (generate_word_token(input));
}

Token	*generate_word_token(const char **input)
{
	const char	*start;
	bool		sf;
	bool		df;

	start = *input;
	sf = false;
	df = false;
	while (**input)
	{
		if (!(sf || df) && ( strchr("<>|", **input) || isspace(**input) ) ) //quote icinde degilsen ayir
			break;
		if (**input == '"' && !sf)
			df = !df;
		if (**input == '\'' && !df)
			sf = !sf;
		(*input)++;
	}
	return (create_token(TOKEN_WORD, input, start));
}

Token	*generate_pr_token(const char **input)
{
	const char	*start;

	start = *input;
	if (**input == '|')
	{
		(*input)++;
		return (create_token(TOKEN_PIPE, input, start));
	}
	if (**input == '<')
	{
		(*input)++;
		if (**input == '<')
		{
			(*input)++;
			return (create_token(TOKEN_LESS_LESS, input, start));
		}
		return (create_token(TOKEN_LESS, input, start));
	}
	if (**input == '>')
	{
		(*input)++;
		if (**input == '>')
		{
			(*input)++;
			return (create_token(TOKEN_GREATER_GREATER, input, start));
		}
		return (create_token(TOKEN_GREATER, input, start));
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

Token	*create_token(TokenType type, const char **input, const char *start)
{
	Token	*token;

	token = (Token *)malloc(sizeof(Token));
	token->type = type;
	token->value = create_word(input, start);
	return (token);
}

void	free_token(Token *token)
{
	if (token->value)
		free(token->value);
	free(token);
}
