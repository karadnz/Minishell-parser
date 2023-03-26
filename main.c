/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaraden <mkaraden@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 16:09:19 by mkaraden          #+#    #+#             */
/*   Updated: 2023/03/25 16:33:46 by mkaraden         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


int main()
{
    const char *input = "ls -l | grep \"txt\" > output.txt";
	//const char *input = "ls -l";
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
int main2(int argc, char **argv)
{
    Token *token;
    do
	{
        token = next_token(&argv[1]);
        printf("Token type: %d, value: %s\n", token->type, token->value);
        //free_token(token);
    } while (token->type != TOKEN_EOF);

    return 0;
}