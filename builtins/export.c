/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmarck <lmarck@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 17:11:46 by lmarck            #+#    #+#             */
/*   Updated: 2025/04/23 22:53:57 by lmarck           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	bi_export(char **arg, t_data *data)
{
	int	i;
	int	error;

	error = 0;
	if (arg[1] == NULL)
		print_export(sort_tab(dup_tab(data->env)));
	else
	{
		i = 1;
		while (arg[i])
		{
			if (!is_valid_var_name(arg[i]))
			{
				put_error(arg[0], arg[i], W_ID);
				error++;
			}
			else
				data->env = modify_env(data->env, arg[i], data);
			i++;
		}
	}
	return (ft_is_zero(error));
}

void	print_export(char **tab)
{
	int	i;
	int	j;

	i = 0;
	while (tab[i])
	{
		j = 0;
		ft_putstr_fd("declare -x ", 1);
		while (tab[i][j] && tab[i][j] != '=')
			ft_putchar_fd(tab[i][j++], 1);
		if (tab[i][j++] == '=')
		{
			ft_putstr_fd("=\"", 1);
			while (tab[i][j])
				ft_putchar_fd(tab[i][j++], 1);
			write(1, "\"", 1);
		}
		write(1, "\n", 1);
		i++;
	}
	free(tab);
}

int	is_valid_var_name(char *arg)
{
	int	i;

	if (!(ft_isalpha(arg[0]) || arg[0] == '_'))
		return (0);
	i = 1;
	while (arg[i] && arg[i] != '=')
	{
		if (!(ft_isalnum(arg[i]) || arg[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}

char	**modify_env(char **tab, char *line, t_data *data)
{
	char	**ntab;
	int		i;

	if (var_exist(tab, line, data))
		return (tab);
	i = 0;
	ntab = ft_calloc(count_line(tab) + 2, sizeof(char *));
	if (!ntab)
		exit_minishell(NULL, "malloc failed", MALLOC_FAIL, data);
	while (tab[i])
	{
		ntab[i] = tab[i];
		i++;
	}
	ntab[i] = ft_strdup(line);
	if (!ntab[i])
	{
		free(tab);
		exit_minishell(NULL, "malloc failed", MALLOC_FAIL, data);
	}
	free(tab);
	return (ntab);
}

int	var_exist(char **tab, char *line, t_data *data)
{
	int		i;
	char	**old;
	char	**new;

	i = 0;
	new = secure_split(line, '=', data);
	while (tab[i])
	{
		old = secure_split(tab[i], '=', data);
		if (new[0] && old[0] && !strcmp(old[0], new[0]))
		{
			if (ft_strchr(line, '='))
			{
				free(tab[i]);
				tab[i] = ft_strdup(line);
				if (!tab[i])
					exit_minishell(NULL, "malloc failed", MALLOC_FAIL, data);
			}
			return (free_tab(old), free_tab(new), 1);
		}
		free_tab(old);
		i++;
	}
	return (free_tab(new), 0);
}
