/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmarck <lmarck@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 15:01:46 by lmarck            #+#    #+#             */
/*   Updated: 2025/04/23 22:54:13 by lmarck           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	bi_unset(char **arg, t_data *data)
{
	int	i;
	int	vi;

	if (!arg[1])
		return (0);
	i = 1;
	while (arg[i])
	{
		vi = find_index(arg[i], data->env);
		if (vi >= 0)
			data->env = delete_line(vi, data->env, data);
		i++;
	}
	return (0);
}

int	find_index(char *arg, char **env)
{
	int	i;
	int	len;

	i = 0;
	len = ft_strlen(arg);
	while (env[i])
	{
		if (!ft_strncmp(arg, env[i], len)
			&& (env[i][len] == '=' || env[i][len] == '\0'))
			return (i);
		i++;
	}
	return (-1);
}

char	**delete_line(int line, char **tab, t_data *data)
{
	int		i;
	int		j;
	char	**ntab;

	i = 0;
	j = 0;
	ntab = ft_calloc(count_line(tab), sizeof(char *));
	if (!ntab)
		exit_minishell(NULL, "malloc fail", MALLOC_FAIL, data);
	while (tab[i])
	{
		if (i == line)
		{
			i++;
			continue ;
		}
		ntab[j] = tab[i];
		j++;
		i++;
	}
	free(tab[line]);
	free(tab);
	return (ntab);
}
