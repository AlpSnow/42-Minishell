/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmarck <lmarck@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 12:48:13 by lmarck            #+#    #+#             */
/*   Updated: 2025/05/10 16:47:49 by lmarck           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	bi_pwd(char **arg, t_data *data)
{
	char	*pwd;

	if (arg[1] && arg[1][0] == '-')
		return (put_error(arg[0], arg[1], ERROR_OPTION), 2);
	pwd = get_env("PWD", data->env);
	if (!pwd)
	{
		pwd = getcwd(NULL, 0);
		if (!pwd)
		{
			ft_putstr_fd("pwd: error retrieving current directory \n", 2);
			return (1);
		}
		if (printf("%s\n", pwd) < 0)
		{
			free(pwd);
			exit_minishell(NULL, "write fail", WRITE_FAIL, data);
		}
		return (free(pwd), 0);
	}
	if (ft_putstrs_fd(get_env("PWD", data->env), 1) < 0)
		exit_minishell(NULL, "write fail", WRITE_FAIL, data);
	if (write(1, "\n", 1) < 0)
		exit_minishell(NULL, "write fail", WRITE_FAIL, data);
	return (0);
}
