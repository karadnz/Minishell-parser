/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaraden <mkaraden@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 22:00:58 by mkaraden          #+#    #+#             */
/*   Updated: 2023/04/05 04:46:22 by mkaraden         ###   ########.fr       */
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
		j = -1;
		while (++j < iter->inf_count)
			printf("%s , ", iter->infile[j]);
		printf("\noutfile: ");
		j = -1;
		while (++j < iter->out_count)
			printf("%s , ", iter->outfile[j]);
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
			printf("%d %s , ", iter->s_infile[j]->type, iter->s_infile[j]->name);
		printf("\noutfile: ");
		j = -1;
		while (++j < iter->out_count)
			printf("%d %s , ", iter->s_outfile[j]->type, iter->s_outfile[j]->name);
		iter = iter->next;
		i++;
	}
	printf("\n\n");
}
