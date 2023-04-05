/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaraden <mkaraden@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 15:49:54 by mkaraden          #+#    #+#             */
/*   Updated: 2023/04/05 04:21:08 by mkaraden         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//bir kez load calistirilacak
//value almak icin get_env_val
//array oalrak alamk icin get env arr
//update delete zart zort
//eklenirken basa eklenir

EnvNode	*load_environment(char *envp[])
{
	EnvNode	*head;
	char	*key;
	char	*value;
	char	*full;
	int		i;

	head = NULL;
	i = 0;
	while (envp[i] != NULL)
	{
		key = strtok(envp[i], "=");
		value = strtok(NULL, "=");
		full = envp[i];
		add_env_node(&head, key, value);
		i++;
	}
	return (head);
}

char	*get_env_val(const char *key)
{
	EnvNode	*current;

	current = find_env_node(env_list, key);
	if (!current)
	{
		printf("ENV VALUE DOESNT EXIST FOR KEY: %s", key);
		return (NULL);
	}
	return (current->value);
}

char	**get_env_arr(EnvNode *head)
{
	char	**rt;
	int		size;
	int		i;
	EnvNode	*current;

	current = head;
	size = 0;
	i = 0;
	while (current != NULL)
	{
		size++;
		current = current->next;
	}
	rt = (char **)malloc(sizeof(char *) * (size + 1));
	current = head;
	while (current != NULL)
	{
		rt[i] = join_env(current->key, current->value);
		i++;
		current = current->next;
	}
	rt[i] = 0;
	return (rt);
}

void	update_env_node(EnvNode *head, const char *key, const char *new_value)
{
	EnvNode	*current;

	current = head;
	while (current != NULL)
	{
		if (strcmp(current->key, key) == 0)
		{
			free(current->value);
			current->value = strdup(new_value);
			return ;
		}
		current = current->next;
	}
}

void	delete_env_node(EnvNode **head, const char *key)
{
	EnvNode	*current;
	EnvNode	*prev;

	current = *head;
	prev = NULL;
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
			return ;
		}
		prev = current;
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

	char **ar =get_env_arr(env_list);
	printf("HIT \n\n\n");
	printDoubleArr(ar);

	free_list(env_list);
	return 0;
}*/