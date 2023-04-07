/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaraden <mkaraden@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 16:09:19 by mkaraden          #+#    #+#             */
/*   Updated: 2023/04/07 16:02:34 by mkaraden         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	Node	*head;
	char	*input;

	env_list = load_environment(envp);
	printf("Original environment variables:\n");
	print_list(env_list);
	head = NULL;
	while (1)
	{
		print_prompt();
		if (take_input(&input))
		{
			/*printf("\n\nOriginal environment variables:\n");
			print_list(env_list);
			printf("\n\n\n\n\n");*/
			system("leaks a.out");
			continue ;
		}
		head = get_parsed((const char **)&input);
		print_parser(head);
		expand_parsed_nodes(head);
		print_parser(head);
		print_parser_with_token(head);
		free_nodes(head);
		//system("leaks a.out");
		//return 0;
		//free(input);
	}
	return (0);
}

/*int main2()
{
    //const char *input = "ls -l | grep txt > output.txt";
	const char *input = "cat << infile | ls -l | wc -l | > outfile";
    Node *ast = parse(input);
    if (ast) {
        printf("AST for command: %s\n", input);
        print_ast(ast, 0);
        free_node(ast);
    } else {
        printf("Error parsing command: %s\n", input);
    }
    return 0;
}
int main3(int argc, char **argv)
{
    Token *token;
    do
	{
        token = next_token(&argv[1]);
        printf("Token type: %d, value: %s\n", token->type, token->value);
        //free_token(token);
    } while (token->type != TOKEN_EOF);

    return 0;
}*/