/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaraden <mkaraden@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/29 15:09:21 by mkaraden          #+#    #+#             */
/*   Updated: 2023/04/07 16:08:08 by mkaraden         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


void *ft_realloc(void *ptr, size_t old_size, size_t new_size) //size_t in bytes
{
    void	*rt;
    int		msize;
	
    msize = old_size;
    rt = malloc(new_size);
    rt = memcpy(rt, ptr, msize);
    free(ptr);
    return	(rt);
}

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
			free(iter->s_infile[j]->name);
			free(iter->s_infile[j]);
		}
		free(iter->s_infile);
		j = -1;
		while (++j < iter->out_count)
		{
			free(iter->s_outfile[j]->name);
			free(iter->s_outfile[j]);
		}
		j = -1;
		while (++j < iter->inf_count)
		{
			free(iter->infile[j]);
		}
		free(iter->infile);
		j = -1;
		while (++j < iter->out_count)
		{
			free(iter->outfile[j]);
		}
		free(iter->outfile);
		free(iter);
		iter = next;
	}
		
		
}
