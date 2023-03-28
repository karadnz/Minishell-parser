/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaraden <mkaraden@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 04:33:35 by mkaraden          #+#    #+#             */
/*   Updated: 2023/03/28 06:16:11 by mkaraden         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_prompt()
{
	printf("mkaraden@minishell$ ");
	
}

int takeInput(char** str)
{
    char* buf;
  
    buf = readline("\n>>> ");
    if (strlen(buf) != 0) {
		*str = (char *)malloc(sizeof(char) * (strlen(buf) + 1));
        add_history(buf);
        strcpy(*str, buf);
        return 0;
    } else {
        return 1;
    }
}