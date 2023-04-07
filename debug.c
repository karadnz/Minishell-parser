/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaraden <mkaraden@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 22:00:58 by mkaraden          #+#    #+#             */
/*   Updated: 2023/04/07 16:15:21 by mkaraden         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//printler burda

void	print_list(EnvNode *head)
{
	EnvNode	*current;

	current = head;
	while (current != NULL)
	{
		printf("%s=%s\n", current->key, current->value);
		current = current->next;
	}
}

void	print_double_arr(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
	{
		printf("%s \n", arr[i]);
		i++;
	}
}

void	print_parser(Node *head)
{
	Node	*iter;
	int		i;
	int		j;

	iter = head;
	i = 0;
	while (iter != NULL)
	{
		printf("\n\n\nNode: %d \nargs: ", i);
		j = -1;
		while (++j < iter->arg_count)
			printf("%s , ", iter->args[j]);
		printf("\ninfile: ");
		iter = iter->next;
		i++;
	}
	printf("\n\n");
}

void	print_parser_with_token(Node *head)
{
	Node	*iter;
	int		i;
	int		j;

	iter = head;
	i = 0;
	while (iter != NULL)
	{
		printf("\n\n\nNode: %d \nargs: ", i);
		j = -1;
		while (++j < iter->arg_count)
			printf("%s , ", iter->args[j]);
		printf("\ninfile: ");
		j = -1;
		while (++j < iter->inf_count)
			printf("%d %s , ", iter->infile[j]->type, iter->infile[j]->name);
		printf("\noutfile: ");
		j = -1;
		while (++j < iter->out_count)
			printf("%d %s , ", iter->outfile[j]->type, iter->outfile[j]->name);
		iter = iter->next;
		i++;
	}
	printf("\n\n");
}
