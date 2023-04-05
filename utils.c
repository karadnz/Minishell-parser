/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaraden <mkaraden@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/29 15:09:21 by mkaraden          #+#    #+#             */
/*   Updated: 2023/04/05 04:27:35 by mkaraden         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	null_terminate_arrs(Node *iter)
{
	iter->args = (char **)realloc(iter->args,
			(iter->arg_count + 1) * sizeof(char *));
	iter->infile = (char **)realloc(iter->infile,
			(iter->inf_count + 1) * sizeof(char *));
	iter->outfile = (char **)realloc(iter->outfile,
			(iter->out_count + 1) * sizeof(char *));
	iter->args[iter->arg_count] = 0;
	iter->infile[iter->inf_count] = 0;
	iter->outfile[iter->out_count] = 0;
}

void	free_token(Token *token)
{
	if (token->value)
		free(token->value);
	free(token);
}
