/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaraden <mkaraden@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/05 03:03:38 by mkaraden          #+#    #+#             */
/*   Updated: 2023/04/05 03:07:39 by mkaraden         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

EnvNode	*create_env_node(const char *key, const char *value)
{
	EnvNode	*node;

	node = (EnvNode *)malloc(sizeof(EnvNode));
	node->key = strdup(key);
	node->value = strdup(value);
	node->next = NULL;
	return (node);
}

void	add_env_node(EnvNode **head, const char *key, const char *value)
{
	EnvNode	*new_node;

	new_node = create_env_node(key, value);
	if (*head == NULL)
	{
		*head = new_node;
		return ;
	}
	new_node->next = *head;
	*head = new_node;
}

EnvNode	*find_env_node(EnvNode *head, const char *key)
{
	EnvNode	*current;

	current = head;
	while (current != NULL)
	{
		if (strcmp(current->key, key) == 0)
		{
			return (current);
		}
		current = current->next;
	}
	return (NULL);
}

void	free_list(EnvNode *head)
{
	EnvNode	*current;
	EnvNode	*next;

	current = head;
	while (current != NULL)
	{
		next = current->next;
		free(current->key);
		free(current->value);
		free(current);
		current = next;
	}
}

char	*join_env(const char *key, const char *value)
{
	size_t	klen;
	size_t	vlen;
	size_t	result_length;
	char	*result;

	klen = strlen(key);
	vlen = strlen(value);
	result_length = klen + vlen + 2;
	result = (char *)malloc(result_length * sizeof(char));
	if (!result)
	{
		printf("Memory allocation failed\n");
		return (NULL);
	}
	memcpy(result, key, klen);
	result[klen] = '=';
	memcpy(result + klen + 1, value, vlen + 1);
	return (result);
}
