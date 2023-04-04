/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaraden <mkaraden@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 04:33:35 by mkaraden          #+#    #+#             */
/*   Updated: 2023/04/05 02:37:31 by mkaraden         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_prompt(void)
{
	printf("mkaraden@minishell$");
}

int	take_input(char **str)
{
	char	*buf;

	buf = readline(" ");
	if (strlen(buf) != 0)
	{
		*str = (char *)malloc(sizeof(char) * (strlen(buf) + 1));
		add_history(buf);
		strcpy(*str, buf);
		return (0);
	}
	else
	{
		return (1);
	}
}
