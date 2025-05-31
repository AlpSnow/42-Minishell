/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmarck <lmarck@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 21:09:14 by lmarck            #+#    #+#             */
/*   Updated: 2025/05/10 14:51:00 by lmarck           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	exit_bi_exit(int ret, t_data *data);
long int	ft_atoi_with_validation(const char *nptr, int *is_valid);

void	bi_exit(char **arg, t_data *data)
{
	int			v;
	int			i;
	long int	ret;

	i = 0;
	if (count_line(arg) > 2)
	{
		write (2, "minishell: exit: too many arguments\n", 37);
		exit_bi_exit(1, data);
	}
	while (arg[++i])
	{
		ret = ft_atoi_with_validation(arg[i], &v);
		if (!v)
		{
			write (2, "minishell: exit: ", 18);
			ft_putstr_fd(arg[1], 2);
			write (2, ": numeric argument required\n", 29);
			exit_bi_exit(2, data);
		}
	}
	if (arg[1] && v)
		exit_bi_exit(ret % 256, data);
	else
		exit_bi_exit(0, data);
}

long int	ft_atoi_with_validation(const char *nptr, int *is_valid)
{
	int		i;
	int		sign;
	long	result;

	i = 0;
	sign = 1;
	result = 0;
	*is_valid = 1;
	while (nptr[i] == 32 || (nptr[i] >= 9 && nptr[i] <= 13))
		i++;
	if (nptr[i] == '+' || nptr[i] == '-')
	{
		if (nptr[i++] == '-')
			sign = sign * -1;
	}
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		result = (result * 10) + (nptr[i++] - 48);
		if ((sign == 1 && result > INT_MAX)
			|| (sign == -1 && (-1 * result) < INT_MIN))
			return (*is_valid = 0, 0);
	}
	if (nptr[i] != '\0')
		*is_valid = 0;
	return (sign * result);
}

static void	exit_bi_exit(int ret, t_data *data)
{
	if (isatty(1) && isatty(0))
		write(1, "exit\n", 5);
	if (isatty(1))
		free_all(data, 0);
	else
		free_all(data, 1);
	exit(ret);
}
