/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmarck <lmarck@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 18:37:05 by lmarck            #+#    #+#             */
/*   Updated: 2025/04/23 22:53:38 by lmarck           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	bi_env(char **arg, t_data *data)
{
	int	i;

	if (arg[1])
	{
		put_error(arg[0], arg[1], ERROR_ARG);
		return (127);
	}
	i = 0;
	while (data->env[i])
	{
		if (ft_strchr(data->env[i], (int) '='))
		{
			if (ft_putstrs_fd(data->env[i], 1) < 0
				|| write (1, "\n", 1) < 0)
				exit_minishell(NULL, "write fail", WRITE_FAIL, data);
		}
		i++;
	}
	return (0);
}
