/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaraden <mkaraden@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/29 15:09:21 by mkaraden          #+#    #+#             */
/*   Updated: 2023/04/07 17:36:37 by mkaraden         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//bir fazla bsize kadar yer ayirir
void	*ft_realloc(void *ptr, size_t b_amount, size_t b_size)
{
	void	*rt;
	int		msize;

	msize = b_amount * b_size;
	rt = malloc((b_amount + 1) * b_size);
	rt = memcpy(rt, ptr, msize);
	free(ptr);
	return (rt);
}

void	null_terminate_arrs(Node *iter)
{
	iter->args = (char **)realloc(iter->args,
			(iter->arg_count + 1) * sizeof(char *));
	iter->infile = (s_file **)ft_realloc(iter->infile,
			(iter->inf_count), sizeof(s_file *));
	iter->outfile = (s_file **)ft_realloc(iter->outfile,
			(iter->out_count), sizeof(s_file *));
	iter->args[iter->arg_count] = 0;
	iter->infile[iter->inf_count] = 0;
	iter->outfile[iter->out_count] = 0;
}

//to prevent double free
void	free_token(Token **token)
{
	if (!(token))
	{
		printf("HIT \n\n\n\n\n\n");
		return ;
	}
	if ((*token)->value)
		free((*token)->value);
	free(*token);
	*token = 0;
}

void	free_nodes(Node *head)
{
	Node	*iter;
	Node	*next;
	int		j;

	iter = head;
	while (iter != NULL)
	{
		next = iter->next;
		j = -1;
		while (++j < iter->arg_count)
			free(iter->args[j]);
		free(iter->args);
		j = -1;
		while (++j < iter->inf_count)
		{
			free(iter->infile[j]->name);
			free(iter->infile[j]);
		}
		free(iter->infile);
		j = -1;
		while (++j < iter->out_count)
		{
			free(iter->outfile[j]->name);
			free(iter->outfile[j]);
		}
		free(iter->outfile);
		free(iter);
		iter = next;
	}
}
