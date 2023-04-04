/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaraden <mkaraden@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 16:09:19 by mkaraden          #+#    #+#             */
/*   Updated: 2023/03/31 15:52:58 by mkaraden         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"



int main(int argc, char **argv, char **envp)
{

	EnvNode *env_list = load_environment(envp);

	printf("Original environment variables:\n");
	print_list(env_list);
    
	char *input;

	//return (0);

	while(1)
	{
		print_prompt();
		if (takeInput(&input))
		{
			printf("\n\nOriginal environment variables:\n");
			print_list(env_list);
			printf("\n\n\n\n\n");

			continue;
		}


		Node *head = parse_main((const char **)&input);
		print_parser(head);
	}

    return 0;
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