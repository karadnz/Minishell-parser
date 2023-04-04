/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaraden <mkaraden@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 16:29:45 by mkaraden          #+#    #+#             */
/*   Updated: 2023/04/05 01:19:21 by mkaraden         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *get_expanded(const char *input)
{
	size_t len = strlen(input);
	size_t expanded_len = len;
	char *expanded = (char *)malloc(expanded_len + 1);
	if (!expanded)
	{
		return NULL;
	}

	size_t write_index = 0;
	bool single_quote = false;
	bool double_quote = false;

	for (size_t i = 0; i < len; i++)
	{
		if (input[i] == '\'' && !double_quote)
		{
			single_quote = !single_quote;
			continue;
		}

		if (input[i] == '\"' && !single_quote)
		{
			double_quote = !double_quote;
			continue;
		}

		if (input[i] == '$' && !single_quote)
		{
			size_t var_start = i + 1;
			size_t var_len = 0;

			while (input[var_start + var_len] != '\0' && (isalnum(input[var_start + var_len]) || input[var_start + var_len] == '_'))
			{
				var_len++;
			}

			char *var_name = (char *)malloc(var_len + 1);
			strncpy(var_name, input + var_start, var_len);
			var_name[var_len] = '\0';

			//char *var_value = getenv(var_name);
			char *var_value = get_env_val(var_name);
			if (var_value)
			{
				size_t var_value_len = strlen(var_value);
				expanded_len += var_value_len - (1 + var_len);
				printf("%s %s %d \n\n\n\n", var_name, var_value, strlen(var_value) - var_len);
				expanded = (char *)realloc(expanded, expanded_len + 1);

				strcpy(expanded + write_index, var_value);
				write_index += var_value_len;
			}

			free(var_name);
			i += var_len;
		}
		else
		{
			expanded[write_index++] = input[i];
		}
	}

	expanded[write_index] = '\0';
	return expanded;
}

void	expand_parsed_nodes(Node *head)
{
	int		i;
	Node	*iter;

	iter = head;
	i = 0;
	while (iter != NULL)
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
		iter = iter->next;
	}
}

/*int main()
{
	char input[] = "    \'\"\'\"l\"=l\"yunus--> $PWD\"\'\"\'  ";
	char *expanded = get_expanded(input);
	printf("Original string: %s\n", input);
	printf("Expanded string: %s\n", expanded);
	free(expanded);
	return 0;
}*/