/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmarck <lmarck@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 14:49:03 by lmarck            #+#    #+#             */
/*   Updated: 2025/05/08 16:40:10 by lmarck           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	is_n_option(char *arg);

int	bi_echo(char **arg, int i, t_data *data)
{
	int	n;

	(void)data;
	n = 0;
	while (arg[i] && is_n_option(arg[i]))
	{
		n = 1;
		i++;
	}
	while (arg[i])
	{
		if (ft_putstrs_fd(arg[i], 1) < 0)
			return (error_perror(arg[0]), 1);
		if (arg[i + 1] && ft_putstrs_fd(" ", 1) < 0)
			return (error_perror(arg[0]), 1);
		i++;
	}
	if (!n)
	{
		if (write(1, "\n", 1) < 0)
			return (error_perror(arg[0]), 1);
	}
	return (0);
}

static int	is_n_option(char *arg)
{
	int	i;

	i = 0;
	if (!arg || arg[i] != '-')
		return (0);
	i++;
	if (arg[i] == '\0')
		return (0);
	while (arg[i] == 'n')
		++i;
	return (arg[i] == '\0');
}
/* int	bi_echo(char **arg, int i, t_data *data)
{
	int	n;

	n = 0;
	while (arg[i] && is_n_option(arg[i]))
	{
		n = 1;
		i++;
	}
	while (arg[i])
	{
		if (ft_putstrs_fd(arg[i], 1) < 0)
			exit_minishell(NULL, "write fail", WRITE_FAIL, data);
		if (arg[i + 1] && ft_putstrs_fd(" ", 1) < 0)
			exit_minishell(NULL, "write fail", WRITE_FAIL, data);
		i++;
	}
	if (!n)
	{
		if (write(1, "\n", 1) < 0)
			return (-1);
	}
	return (0);
} */
