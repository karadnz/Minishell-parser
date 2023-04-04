
/*Token	*next_token2(const char **input)
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
	case '"':
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
		//return (create_token(TOKEN_SINGLE_QUOTE, "'"));
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
}*/