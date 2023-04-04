/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaraden <mkaraden@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/29 15:09:21 by mkaraden          #+#    #+#             */
/*   Updated: 2023/04/04 23:50:34 by mkaraden         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void *ft_realloc(void *ptr, size_t old_size, size_t new_size) //duzelt
{
	if (new_size == 0)
	{
		free(ptr);
		return NULL;
	}

	if (ptr == NULL)
	{
		return malloc(new_size);
	}

	void *new_ptr = malloc(new_size);
	if (!new_ptr)
	{
		printf("Failed to allocate memory.\n");
		return NULL;
	}

	size_t min_size = old_size < new_size ? old_size : new_size;
	memcpy(new_ptr, ptr, min_size);
	free(ptr);

	return new_ptr;
}

void printDoubleArr(char **arr)
{
	int i;

	i = 0;
	while (arr[i])
	{
		printf("%s \n", arr[i]);
		i++;
	}
}

void print_parser(Node *head)
{
	Node *iter = head;

	int i = 0;
	while(iter != NULL)
	{
		//printf("p\n\n");
		printf("Node: %d \n", i);
		//printf("Type: %d \n", iter->type);
		printf("args: ");
		//printf("p\n\n");
		for (int i = 0; i < iter->arg_count; i++)
		{
			printf("%s , ", iter->args[i]);
			//printf("p\n\n");
		}
		printf("\n");

		printf("infile: ");
		//printf("p\n\n");
		for (int i = 0; i < iter->inf_count; i++)
		{
			printf("%s , ", iter->infile[i]);
			//printf("p\n\n");
		}
		printf("\n");

		printf("outfile: ");
		//printf("p\n\n");
		for (int i = 0; i < iter->out_count; i++)
		{
			printf("%s , ", iter->outfile[i]);
			//printf("p\n\n");
		}
		printf("\n\n\n");
		//printf("p\n\n");
		iter=iter->next;
		i++;
	}
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