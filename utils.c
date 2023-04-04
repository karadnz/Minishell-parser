/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaraden <mkaraden@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/29 15:09:21 by mkaraden          #+#    #+#             */
/*   Updated: 2023/04/04 22:55:34 by mkaraden         ###   ########.fr       */
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