/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaraden <mkaraden@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 16:29:45 by mkaraden          #+#    #+#             */
/*   Updated: 2023/04/05 02:13:29 by mkaraden         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_expanded(const char *str)
{
	exp_stsh	*stsh; // XXXXX
	bool		sf;
	bool		df;
	
	stsh = get_stsh(str);
	sf = false;
	df = false;
	expand_stsh(str, stsh, sf, df);
	return(stsh->rt);
}

void	expand_stsh(const char *str, exp_stsh *stsh, bool sf, bool df)
{
	while(str[stsh->src_i])
	{
		if (str[stsh->src_i] == '"' && !sf)
			df = !df;
		else if (str[stsh->src_i] == '\'' && !df)
			sf = !sf;
		else if (str[stsh->src_i] == '$' && !sf)
			dollarize(str, stsh);
		else
		{
			stsh->rt[stsh->rt_i++] = str[stsh->src_i];
		}
		stsh->src_i++;
	}
	stsh->rt[stsh->rt_i] = '\0';	
}

void	dollarize(const char *str, exp_stsh *stsh)
{
	int		var_start;
	int		var_len;
	char	*var_name;
	char	*var_val;
	
	var_start = stsh->src_i + 1;
	var_len = 0;
	while (str[var_start + var_len] != '\0' && (isalnum(str[var_start + var_len]) || str[var_start + var_len] == '_'))
	{
		var_len++;
	}
	var_name = (char *)malloc(var_len + 1);
	strncpy(var_name, str + var_start, var_len);
	var_name[var_len] = '\0';
	var_val = get_env_val(var_name);
	if (var_val)
	{
		stsh->rt_len += strlen(var_val) - (1 + var_len);
		stsh->rt = (char *)realloc(stsh->rt, stsh->rt_len + 1);
		strcpy(stsh->rt + stsh->rt_i, var_val);
		stsh->rt_i += strlen(var_val);
	}
	stsh->src_i += var_len;
}

exp_stsh	*get_stsh(const char *str)
{
	exp_stsh	*rt;

	rt = (exp_stsh *)malloc(sizeof(exp_stsh));
	rt->rt_i = 0;
	rt->src_i = 0;
	rt->src_len = strlen(str);
	rt->rt_len = rt->src_len;
	rt->rt = (char *)malloc(sizeof(char) * (rt->rt_len + 1));
	return (rt);
}

void	expand_parsed_nodes(Node *head)
{
	int		i;
	Node	*iter;

	iter = head;
	i = 0;
	while (iter != NULL)
	{
		i = 0;
		while (i < iter->arg_count)
		{
			iter->args[i] = get_expanded(iter->args[i]);
			i++;
		}
		i = 0;
		while (i < iter->inf_count)
		{
			iter->infile[i] = get_expanded(iter->infile[i]);
			i++;
		}
		i = 0;
		while (i < iter->out_count)
		{
			iter->outfile[i] = get_expanded(iter->outfile[i]);
			i++;
		}
		iter = iter->next;
	}
}

/*int main()
{
	char input[] = "    \'\"\'\"l\"=l\"yunus--> $PWD\"\'\"\'  ";
	char *expanded = get_expanded(input);
	printf("Original string: %s\n", input);
	printf("Expanded string: %s\n", expanded);
	free(expanded);
	return 0;
}*/