/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaraden <mkaraden@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 15:49:54 by mkaraden          #+#    #+#             */
/*   Updated: 2023/03/31 16:06:35 by mkaraden         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

EnvNode *create_env_node(const char *key, const char *value)
{
	EnvNode *node = (EnvNode *)malloc(sizeof(EnvNode));
	node->key = strdup(key);
	node->value = strdup(value);
	node->next = NULL;
	return node;
}

void add_env_node(EnvNode **head, const char *key, const char *value) // basa ekliyor
{
	EnvNode *new_node = create_env_node(key, value);
	if (*head == NULL)
	{
		*head = new_node;
		return;
	}
	new_node->next = *head;
	*head = new_node;
}

void delete_env_node(EnvNode **head, const char *key)
{
	EnvNode *current = *head;
	EnvNode *prev = NULL;

	while (current != NULL)
	{
		if (strcmp(current->key, key) == 0)
		{
			if (prev == NULL)
			{
				*head = current->next;
			}
			else
			{
				prev->next = current->next;
			}
			free(current->key);
			free(current->value);
			free(current);
			return;
		}
		prev = current;
		current = current->next;
	}
}

void update_env_node(EnvNode *head, const char *key, const char *new_value)
{
	EnvNode *current = head;

	while (current != NULL)
	{
		if (strcmp(current->key, key) == 0)
		{
			free(current->value);
			current->value = strdup(new_value);
			return;
		}
		current = current->next;
	}
}

EnvNode *find_env_node(EnvNode *head, const char *key)
{
	EnvNode *current = head;

	while (current != NULL)
	{
		if (strcmp(current->key, key) == 0)
		{
			return current;
		}
		current = current->next;
	}

	return NULL;
}

EnvNode *load_environment(char *envp[])
{
	EnvNode *head = NULL;

	for (int i = 0; envp[i] != NULL; i++)
	{
		char *key = strtok(envp[i], "=");
		char *value = strtok(NULL, "=");
		add_env_node(&head, key, value);
	}

	return head;
}

void free_list(EnvNode *head)
{
	EnvNode *current = head;

	while (current != NULL)
	{
		EnvNode *next = current->next;
		free(current->key);
		free(current->value);
		free(current);
		current = next;
	}
}

void print_list(EnvNode *head)
{
	EnvNode *current = head;

	while (current != NULL)
	{
		printf("%s=%s\n", current->key, current->value);
		current = current->next;
	}
}

/*int main(int argc, char *argv[], char *envp[])
{
	EnvNode *env_list = load_environment(envp);

	printf("Original environment variables:\n");
	print_list(env_list);

	// Add a new variable
	add_env_node(&env_list, "NEW_VARIABLE", "new_value");
	printf("\nAfter adding NEW_VARIABLE:\n");
	print_list(env_list);

	// Update an existing variable
	update_env_node(env_list, "PATH", "/new/path");
	printf("\nAfter updating PATH:\n");
	print_list(env_list);

	// Delete a variable
	delete_env_node (&env_list, "NEW_VARIABLE");
	printf("\nAfter deleting NEW_VARIABLE:\n");
	print_list(env_list);

	free_list(env_list);
	return 0;
}*/
